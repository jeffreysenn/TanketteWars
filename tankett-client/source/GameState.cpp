#include "GameState.h"
#include "StateIdentifiers.h"
#include "Context.h"
#include "ClientStateStack.h"
#include <SFML/Window/Event.hpp>

namespace client
{
GameState::GameState()
try : mWorld()
, mFrameNum(0)
{
	::std::unique_ptr<PlayerController> controller(new PlayerController(0, true, Context::getInstance().window));
	mPlayerControllers.push_back(std::move(controller));
	mPlayerControllers.back()->spawnTank_server(mWorld.getTankManager());
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

bool GameState::update(float deltaSeconds)
{
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