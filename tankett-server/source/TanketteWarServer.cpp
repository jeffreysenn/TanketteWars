#include "TanketteWarServer.h"

#include "WaitingState.h"
#include "GameState.h"
#include "EndState.h"
#include <SFML/System/Clock.hpp>

namespace mw
{
Game* Game::create()
{
	return new ::server::TanketteWarServer();
}
}

namespace server
{
TanketteWarServer::TanketteWarServer()
{
	init();
	Context& context = Context::getInstance();
	context.networkManager = &mNetworkManager;
	context.stack = &mStateStack;
}

void TanketteWarServer::init()
{
	registerStates();
	mStateStack.pushState(GAME_STATE::WAITING_FOR_PLAYER);
}

void TanketteWarServer::registerStates()
{
	mStateStack.registerState<WaitingState>(GAME_STATE::WAITING_FOR_PLAYER);
	mStateStack.registerState<EndState>(GAME_STATE::ROUND_END);
	mStateStack.registerState<GameState>(GAME_STATE::ROUND_RUNNING);
}

const sf::Time networkInterval = sf::seconds(1.f / PROTOCOL_SEND_PER_SEC);
void TanketteWarServer::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastSend = sf::Time::Zero;

	while (true)
	{
		sf::Time lastLoopDuration = clock.restart();
		// network fixed send
		timeSinceLastSend += lastLoopDuration;

		while (timeSinceLastSend > networkInterval)
		{
			mNetworkManager.send();
			timeSinceLastSend -= networkInterval;
		}

		mNetworkManager.receive();

		mStateStack.processMessages();
	}
}


}