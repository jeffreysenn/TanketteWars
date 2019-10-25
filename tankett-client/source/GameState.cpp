#include "GameState.h"
#include "Controllers/PlayerController.h"
#include "StateIdentifiers.h"
#include "ClientContext.h"
#include "ClientStateStack.h"
#include <SFML/Window/Event.hpp>

GameState::GameState()
try : mWorld()
	, mPlayerController(*ClientContext::getInstance().playerController)
	, mFrameNum(0)
{
}
catch (const std::runtime_error& e)
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
	mPlayerController.handleRealtimeInput(mWorld.getCommandQueue(), mFrameNum);
	return true;
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::handleEvent(const sf::Event & event)
{
	mPlayerController.handleEvent(event, mWorld.getCommandQueue(), mFrameNum);

	if(Input::eventInputCollectionPressed(event, mPauseInputs))
		ClientContext::getInstance().stack->pushState(StateID::Pause);

	return true;
}
