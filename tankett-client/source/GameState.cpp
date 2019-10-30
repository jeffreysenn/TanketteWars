#include "GameState.h"
#include "StateIdentifiers.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "NetworkManager.h"
#include <SFML/Window/Event.hpp>
#include "Tank.h"
#include "Bullet.h"
#include "tankett_debug.h"

namespace client
{
GameState::GameState()
try : mWorld()
, mFrameNum(0)
{}
catch (const std::runtime_error & e)
{
	std::cout << "Exception: " << e.what() << std::endl;
	// making sure the world is constructed successfully
	std::terminate();
}

GameState::~GameState()
{
}

void GameState::processReceivedMessages()
{
	auto& networkManager = *Context::getInstance().networkManager;
	auto& receivedMessages = networkManager.getReceivedMessages();
	for (auto& message : receivedMessages)
	{
		network_message_type type = (network_message_type)message->type_;
		switch (type)
		{
		case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
		{
			message_server_to_client* msgS2C = (message_server_to_client*)message.get();
			if (!msgS2C) break;

			checkNewRemote(msgS2C);
			updateRemoteState(msgS2C);
		}
		break;
		default:
			break;
		}
	}
	networkManager.clearReceivedMessages();
}

void GameState::checkNewRemote(::tankett::message_server_to_client* msgS2C)
{
	auto dataArr = msgS2C->client_data;
	// if new players are connected
	if (mPlayerControllers.size() < msgS2C->client_count)
	{
		for (int i = 0; i < msgS2C->client_count; ++i)
		{
			bool controllerExist = false;
			for (auto& controller : mPlayerControllers)
			{
				if (controller->getID() == dataArr[i].client_id)
				{
					controllerExist = true;
					break;
				}
			}
			if (!controllerExist)
			{
				bool listenToInput = msgS2C->receiver_id == dataArr[i].client_id;
				auto controller = ::std::make_unique<PlayerController>(dataArr[i].client_id, listenToInput, Context::getInstance().window);
				auto pos = ::sf::Vector2f(dataArr[i].position.x_, dataArr[i].position.y_);
				if (listenToInput)
				{
					controller->setNetRole(NetRole::AutonomousProxy);
					mLocalController = controller.get();
					controller->spawnTank_client(mWorld.getTankManager(), pos);
				}
				else
				{
					controller->setNetRole(NetRole::SimulatedProxy);
					controller->spawnTank_client(mWorld.getTankManager(), pos);
				}

				mPlayerControllers.push_back(::std::move(controller));
			}
		}
	}
}

void GameState::updateRemoteState(::tankett::message_server_to_client* msgS2C)
{
	auto dataArr = msgS2C->client_data;
	// update state
	for (int i = 0; i < msgS2C->client_count; ++i)
	{
		auto& data = dataArr[i];
		for (auto& controller : mPlayerControllers)
		{
			// remote controllers
			if (controller->getID() == data.client_id)
			{
				// sync the bullets
				// bullets exist both on server and local: transform
				// bullets exist only on local: destroy
				// bullets exist only on server: spawn
				uint8_t bulletCount = data.bullet_count;
				auto& bullets = data.bullets;
				auto& existingBullets = controller->getBullets();
				::std::map<uint8_t, ::std::pair<::tankett::Bullet*, ::tankett::bullet_data*>> bulletMap{};
				for (int bulletIndex = 0; bulletIndex < bulletCount; ++bulletIndex)
				{
					bulletMap[bullets[bulletIndex].id].second = &bullets[bulletIndex];
				}
				for (auto& existingBullet : existingBullets)
				{
					bulletMap[existingBullet->getID()].first = existingBullet;
				}
				for (auto& it : bulletMap)
				{
					auto& bulletPair = it.second;
					if (bulletPair.first && bulletPair.second)
					{
						bulletPair.first->setPosition(bulletPair.second->position.x_, bulletPair.second->position.y_);
					}
					else if (bulletPair.first && !bulletPair.second)
					{
						::mw::Actor::destroy(bulletPair.first);
					}
					else if (!bulletPair.first && bulletPair.second)
					{
						auto tank = controller->getPossessedTank();
						if(tank)
						{
							Bullet* newBullet = tank->spawnBullet();
							newBullet->setNetRole(NetRole::SimulatedProxy);
							newBullet->setID(bulletPair.second->id);
							newBullet->setPosition(bulletPair.second->position.x_, bulletPair.second->position.y_);
						}
					}
				}

				// sync tank state
				auto tank = controller->getPossessedTank();
				if (data.alive)
				{
					::sf::Vector2f pos = ::sf::Vector2f(data.position.x_, data.position.y_);
					float aimAngle = data.angle;
					controller->setTankState(pos, aimAngle);
				}
				else if(tank)
				{
					Actor::destroy(tank);
				}

				// input prediction
				if (controller.get() == mLocalController)
				{
					for (uint32_t inputIndex = msgS2C->input_number + 1; inputIndex < mFrameNum; ++inputIndex)
					{
						auto& input = controller->getInputBuffer()[inputIndex];
						float deltaSeconds = 1.f / 60.f;
						controller->updateTank(input.up, input.down, input.left, input.right, input.fire, input.angle, deltaSeconds);
					}
				}
			}
		}
	}
}

constexpr uint32_t inputMsgPackNum = 10;
constexpr size_t maxMsgNum = 30;
void GameState::pushMessages()
{
	if (!mLocalController)
		return;

	auto& networkManager = *Context::getInstance().networkManager;
	if (networkManager.getSendMessageQueue().size() >= maxMsgNum) return;

	auto inputBuffer = mLocalController->getInputBuffer();
	uint32_t inputBegin = mFrameNum - inputMsgPackNum;
	if (inputBuffer.find(inputBegin) != inputBuffer.end())
	{
		for (uint32_t inputKey = inputBegin; inputKey < mFrameNum; ++inputKey)
		{
			auto inputMessage = ::std::make_unique<message_client_to_server>();
			inputMessage->input_number = inputKey;
			auto& inputValue = inputBuffer[inputKey];
			inputMessage->set_input(inputValue.fire, inputValue.right, inputValue.left, inputValue.down, inputValue.up);
			inputMessage->turret_angle = inputValue.angle;
			networkManager.pushMessage(::std::move(inputMessage));
		}
	}
}

bool GameState::update(float deltaSeconds)
{
	processReceivedMessages();

	++mFrameNum;
	mWorld.update(deltaSeconds);
	if (Context::getInstance().isWindowFocused)
	{
		for (auto& playerController : mPlayerControllers)
		{
			playerController->handleRealtimeInput(mFrameNum);
		}
	}

	pushMessages();
	return true;
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::handleEvent(const ::sf::Event& event)
{
	for (auto& playerController : mPlayerControllers)
	{
		playerController->handleEvent(event, mFrameNum);
	}

	if (Input::eventInputCollectionPressed(event, mPauseInputs))
		Context::getInstance().stack->pushState(StateID::Pause);

	return true;
}
}