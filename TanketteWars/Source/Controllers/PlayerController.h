#pragma once

#include "../Commands/Command.h"
#include "../Input/Input.h"

#include <map>
#include <vector>

namespace sf
{
	class Event;
	class RenderWindow;
}

class PlayerController
{
public:
	PlayerController(const sf::RenderWindow& window);

	void handleEvent(const sf::Event& event, class CommandQueue& commandQueue);
	void handleRealtimeInput(class CommandQueue& commandQueue);

	sf::Vector2f getMousePosition() const;

private:
	enum class Action
	{
		Up,
		Down,
		Left,
		Right,
		Fire,
	};

	struct GameInput
	{
		Input::InputCollection inputCollection;
		bool bIsRealTime;
	};

private:
	void bindInputs();
	void bindActions();

private:
	Command mCommand;

	std::map<Action, GameInput> mInputBinding;
	std::map<Action, Command> mActionBinding;
	Command mMousePosCommand;

	const sf::RenderWindow& mWindow;
};

