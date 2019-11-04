#include "GameState.h"
#include "StateIdentifiers.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "NetworkManager.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Tank.h"
#include "Bullet.h"
#include "tankett_debug.h"
#include "Actors/CameraActor.h"
#include "Context.h"
#include <Helpers/Helper.h>

namespace client
{
GameState::GameState()
try : mWorld()
, mFrameNum(0)
{
	mRoundTimerText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mRoundTimerText.setString("90");
	mRoundTimerText.setCharacterSize(60);
	mRoundTimerText.setPosition(Context::getInstance().window->getSize().x / 2.f, 50.f);
	::mw::helper::Graphics::centreOrigin(mRoundTimerText);

	mCooldownText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mCooldownText.setString("1000");
	mCooldownText.setCharacterSize(14);
	mCooldownText.setPosition(Context::getInstance().window->getSize().x / 2.f, Context::getInstance().window->getSize().y / 2.f - 50.f);
	::mw::helper::Graphics::centreOrigin(mCooldownText);

	mScoreBoard.setPosition(Context::getInstance().window->getSize().x - 100.f, 50.f);
}
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
		::tankett::network_message_type type = (::tankett::network_message_type)message->type_;
		switch (type)
		{
		case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
		{
			::tankett::message_server_to_client* msgS2C = (::tankett::message_server_to_client*)message.get();
			if (!msgS2C) break;

			checkNewRemote(msgS2C);

			const auto& dataArr = msgS2C->client_data;
			for (int i = 0; i < msgS2C->client_count; ++i)
			{
				const auto& data = dataArr[i];
				// validate prediction immediately
				if (data.client_id == msgS2C->receiver_id)
				{
					validateInputPrediction(data, msgS2C->input_number);

					auto found = mPredictedStates.find(msgS2C->input_number);
					if(found != mPredictedStates.end())
					mPredictedStates.erase(mPredictedStates.begin(), found);
				}
				// we do not update remotes immediately after receiving the message
				else
				{
					mRemoteStates[msgS2C->input_number] = data;
				}
			}

			// we do not update immediately after receiving the message
			// update when the frame num is ahead of the input_num of the message enough
			// to make sure the update rate is stable
			updateState(msgS2C);
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
				auto controller = ::std::make_unique<::tankett::PlayerController>(dataArr[i].client_id, listenToInput, Context::getInstance().window);
				auto pos = ::sf::Vector2f(dataArr[i].position.x_, dataArr[i].position.y_);
				if (listenToInput)
				{
					controller->setNetRole(::mw::NetRole::AutonomousProxy);
					mLocalController = controller.get();
				}
				else
				{
					controller->setNetRole(::mw::NetRole::SimulatedProxy);
				}
				controller->spawnTank_client(mWorld.getTankManager(), pos);
				mPlayerControllers.push_back(::std::move(controller));
			}
		}
	}
}

float destroyedLocalBulletAngle = 0;
void GameState::updateState(::tankett::message_server_to_client* msgS2C)
{
	int remainingTime = (int)msgS2C->round_time;
	mRoundTimerText.setString(::std::to_string(remainingTime));

	auto dataArr = msgS2C->client_data;
	// update state
	for (int i = 0; i < msgS2C->client_count; ++i)
	{
		auto& data = dataArr[i];
		for (auto& controller : mPlayerControllers)
		{
			if (controller->getID() == data.client_id && controller.get() != mLocalController)
			{
				controller->setPing(data.ping);
				controller->setScore(data.eliminations);
				
				syncBulletState(data, *controller.get());

				// sync tank state
				auto tank = controller->getPossessedTank();
				if (data.alive)
				{
					::sf::Vector2f pos = ::sf::Vector2f(data.position.x_, data.position.y_);
					float aimAngle = data.angle;
					controller->setTankState(pos, aimAngle);
				}
				else if (tank)
				{
					Actor::destroy(tank);
				}
			}
		}
	}

}

void GameState::validateInputPrediction(const::tankett::PlayerState& state, uint32_t inputNum)
{
	if (!mLocalController)
		return;

	if (mPredictedStates[inputNum] == state)
		return;

	syncBulletState(state, *mLocalController);

	auto tank = mLocalController->getPossessedTank();
	if (state.alive)
	{
		::sf::Vector2f pos = ::sf::Vector2f(state.position.x_, state.position.y_);
		float aimAngle = state.angle;
		mLocalController->setTankState(pos, aimAngle);
	}
	else if (tank)
	{
		Actor::destroy(tank);
		::mw::SceneGraph* sceneGraph = (::mw::SceneGraph*) mLocalController->getPossessedTank()->getSceneGraph();
		sceneGraph->enforceDestruction(*sceneGraph);
	}

	for (uint32_t inputIndex = inputNum + 1; inputIndex <= mFrameNum; ++inputIndex)
	{
		// break in case client is too behind the server
		if (mFrameNum - inputIndex > 10) break;

		auto& input = mLocalController->getInputBuffer()[inputIndex];
		float deltaSeconds = 1.f / 60.f;
		mLocalController->updateTank(input.up, input.down, input.left, input.right, input.fire, input.angle, deltaSeconds, inputIndex);
		mPredictedStates[inputIndex] = mLocalController->getTankState();
	}
}

void GameState::syncBulletState(const tankett::PlayerState& state, ::tankett::PlayerController& controller)
{
	// sync the bullets
	// bullets exist both on server and local: transform
	// bullets exist only on local: destroy
	// bullets exist only on server: spawn
	uint8_t bulletCount = state.bullet_count;
	const auto& bullets = state.bullets;
	auto& existingBullets = controller.getBullets();
	::std::map<uint8_t, ::std::pair<::tankett::Bullet*, const ::tankett::bullet_data*>> bulletMap{};
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
			auto* tank = controller.getPossessedTank();
			if (tank)
			{
				::tankett::Bullet* newBullet = tank->spawnBullet(tank->getTurretAngle());
				newBullet->setID(bulletPair.second->id);
				newBullet->setPosition(bulletPair.second->position.x_, bulletPair.second->position.y_);
			}
		}
	}
}

constexpr uint32_t redundantInputNum = 10;
constexpr uint32_t maxInputNum = 60;
void GameState::packInput()
{
	if (!mLocalController)
		return;

	auto& networkManager = *Context::getInstance().networkManager;
	auto& inputBuffer = mLocalController->getInputBuffer();

	// pack in redundant input if Queue is empty
	if (networkManager.getSendMessageQueue().size() < redundantInputNum)
	{
		uint32_t inputBegin = mFrameNum - redundantInputNum;
		if (inputBuffer.find(inputBegin) != inputBuffer.end())
		{
			for (uint32_t inputKey = inputBegin; inputKey < mFrameNum; ++inputKey)
			{
				auto& inputValue = inputBuffer[inputKey];
				pushInputMessage(inputValue, inputKey);
			}
		}
	}

	if (networkManager.getSendMessageQueue().size() < 60)
	{
		auto& inputValue = inputBuffer[mFrameNum];
		pushInputMessage(inputValue, mFrameNum);
	}
}

void GameState::pushInputMessage(::tankett::PlayerController::TankInput& inputValue, uint32_t inputNum)
{
	auto inputMessage = ::std::make_unique<::tankett::message_client_to_server>();
	inputMessage->input_number = inputNum;
	inputMessage->set_input(inputValue.fire, inputValue.right, inputValue.left, inputValue.down, inputValue.up);
	inputMessage->turret_angle = inputValue.angle;
	auto& networkManager = *Context::getInstance().networkManager;
	networkManager.pushMessage(::std::move(inputMessage));
}

bool GameState::update(float deltaSeconds)
{
	processReceivedMessages();

	++mFrameNum;

	handleInput();

	mWorld.update(deltaSeconds);

	if (mLocalController)
	{
		mPredictedStates[mFrameNum] = mLocalController->getTankState();
	}

	updateCooldownText();

	packInput();

	updateScoreboard();

	return true;
}

void GameState::handleInput()
{
	if (Context::getInstance().isWindowFocused)
	{
		// set the view to the world camera view to get relative mouse pos for player controller 
		Context::getInstance().window->setView(*Context::getInstance().camera);
		if (mLocalController)
			mLocalController->handleRealtimeInput(mFrameNum);
	}
}

void GameState::updateScoreboard()
{
	// update scoreboard
	for (auto& playerController : mPlayerControllers)
	{
		mScoreBoard.setScoreBoard(playerController->getID(), playerController->getScore(), playerController->getPing());
	}
}

void GameState::updateCooldownText()
{
	if (mLocalController && mLocalController->getPossessedTank())
	{
		auto inversedCooldown = 1000 - mLocalController->getPossessedTank()->getCooldown();
		mCooldownText.setString(::std::to_string(inversedCooldown));
		if (inversedCooldown == 1000)
		{
			mCooldownText.setFillColor(::sf::Color::Green);
		}
		else
		{
			mCooldownText.setFillColor(::sf::Color::Red);
		}
	}
}

void GameState::draw()
{
	mWorld.draw();
	auto& window = Context::getInstance().window;
	window->setView(window->getDefaultView());
	window->draw(mRoundTimerText);
	window->draw(mCooldownText);
	window->draw(mScoreBoard);
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