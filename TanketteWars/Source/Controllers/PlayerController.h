#pragma once

#include "../Commands/Command.h"
#include "../Input/Input.h"
#include "tankett_shared.h"

#include <map>
#include <vector>

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace tankett
{
	struct serverToClientData;
}

class PlayerController
{
public:
	PlayerController();
	PlayerController(sf::RenderWindow& window);

	void handleEvent(const sf::Event& event, class CommandQueue& commandQueue, uint32_t frameNum);
	void handleRealtimeInput(class CommandQueue& commandQueue, uint32_t frameNum);

	sf::Vector2f getMousePosition() const;

	void validateInputPrediction(const tankett::serverToClientData& state, uint32_t frameNum);

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
		bool bIsRealTime{};
	};

private:
	void bindInputs();
	void bindActions();

private:
	Command mMousePosCommand;

	std::map<Action, GameInput> mInputBinding;
	std::map<Action, Command> mActionBinding;
	std::map<uint32_t, std::vector<Command>> mCommandBuffer;
	sf::RenderWindow* mWindow;
};

