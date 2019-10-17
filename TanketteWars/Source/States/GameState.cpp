#include "GameState.h"
#include "../Controllers/PlayerController.h"
#include "StateIdentifiers.h"

#include <SFML/Window/Event.hpp>

GameState::GameState(StateStack & stateStack, const Context & context)
try : State(stateStack, context)
	, mWorld(context)
	, mPlayerController(*context.playerController)
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
	mWorld.update(deltaSeconds);
	mPlayerController.handleRealtimeInput(mWorld.getCommandQueue());
	return true;
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::handleEvent(const sf::Event & event)
{
	mPlayerController.handleEvent(event, mWorld.getCommandQueue());

	if(Input::eventInputCollectionPressed(event, mPauseInputs))
		requestStackPush(StateID::Pause);

	return true;
}
