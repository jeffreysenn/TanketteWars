#include "PlayerController.h"

#include "../Actors/Tank.h"
#include "../Commands/CommandQueue.h"
#include <SFML/Graphics/RenderWindow.hpp>

void upAction(Tank &tank, float)
{
	tank.addDirection(0, -1);
}

void downAction(Tank& tank, float)
{
	tank.addDirection(0, 1);
}

void leftAction(Tank& tank, float)
{
	tank.addDirection(-1, 0);
}

void rightAction(Tank& tank, float)
{
	tank.addDirection(1, 0);
}

sf::Vector2f mousePos;
void mousePosAction(Tank& tank, float)
{
	tank.aimAt(mousePos);
}

void fireAction(Tank& tank, float)
{
	tank.fire();
}

PlayerController::PlayerController(const sf::RenderWindow& window)
	: mWindow(window)
{
	bindInputs();
	bindActions();
}

void PlayerController::handleEvent(const sf::Event & event, CommandQueue & commandQueue)
{
	for (auto &pair : mActionBinding)
	{
		const auto & gameInput = mInputBinding[pair.first];
		if ((!gameInput.bIsRealTime) &&
			Input::eventInputCollectionPressed(event, gameInput.inputCollection))
			commandQueue.push(pair.second);
	}
}

void PlayerController::handleRealtimeInput(CommandQueue &commandQueue)
{
	for (auto &pair : mActionBinding)
	{
		const auto & gameInput = mInputBinding[pair.first];
		if (gameInput.bIsRealTime &&
			Input::inputCollectionPressed(gameInput.inputCollection))
			commandQueue.push(pair.second);
	}

	mousePos = getMousePosition();
	commandQueue.push(mMousePosCommand);
}

sf::Vector2f PlayerController::getMousePosition() const
{
	return mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
}

void PlayerController::bindInputs()
{
	mInputBinding[Action::Up] =
	{
		{
			{Input::Type::Keyboard, sf::Keyboard::W},
			{Input::Type::Keyboard, sf::Keyboard::Up}
		}, true
	};

	mInputBinding[Action::Down] =
	{
		{
			{Input::Type::Keyboard, sf::Keyboard::S},
			{Input::Type::Keyboard, sf::Keyboard::Down}
		}, true
	};

	mInputBinding[Action::Left] =
	{
		{
			{Input::Type::Keyboard, sf::Keyboard::A},
			{Input::Type::Keyboard, sf::Keyboard::Left}
		}, true
	};

	mInputBinding[Action::Right] =
	{
		{
			{Input::Type::Keyboard, sf::Keyboard::D},
			{Input::Type::Keyboard, sf::Keyboard::Right}
		}, true
	};

	mInputBinding[Action::Fire] =
	{
		{{Input::Type::Mouse, sf::Mouse::Left}},
		false
	};
}

void PlayerController::bindActions()
{
	mActionBinding[Action::Up].action = 
		derivedAction<Tank>(upAction);

	mActionBinding[Action::Down].action =
		derivedAction<Tank>(downAction);

	mActionBinding[Action::Left].action =
		derivedAction<Tank>(leftAction);

	mActionBinding[Action::Right].action =
		derivedAction<Tank>(rightAction);

	mActionBinding[Action::Fire].action = 
		derivedAction<Tank>(fireAction);

	for (auto &pair : mActionBinding)
		pair.second.category = CommandCategory::Tank0;

	mMousePosCommand.action = derivedAction<Tank>(mousePosAction);
	mMousePosCommand.category = CommandCategory::Tank0;
}

