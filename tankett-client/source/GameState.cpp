#include "GameState.h"
#include "StateIdentifiers.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "NetworkManager.h"
#include <SFML/Window/Event.hpp>

namespace client
{
GameState::GameState()
try : mWorld()
, mFrameNum(0)
{
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

void GameState::processMessages()
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
						controller->spawnTank_client(mWorld.getTankManager(), pos);
						mPlayerControllers.push_back(::std::move(controller));
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

bool GameState::update(float deltaSeconds)
{
	processMessages();

	++mFrameNum;
	mWorld.update(deltaSeconds);
	if (Context::getInstance().isWindowFocused)
	{
		for (auto& playerController : mPlayerControllers)
		{
			playerController->handleRealtimeInput(mFrameNum);
		}
	}

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