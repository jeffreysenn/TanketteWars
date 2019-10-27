#pragma once

#include "Commands/Command.h"
#include "Commands/CommandCategory.h"
#include "Input/Input.h"

#include <map>
#include <vector>

namespace sf
{
class Event;
class RenderWindow;
}

namespace mw
{
class CommandQueue;
}

using namespace mw;

namespace tankett
{
class PlayerController
{
public:
	PlayerController(CommandCategory category = CommandCategory::None, bool listenToInput = false, ::sf::RenderWindow* window = nullptr);

	void handleEvent(const ::sf::Event& event, CommandQueue& commandQueue, uint32_t frameNum);
	void handleRealtimeInput(CommandQueue& commandQueue, uint32_t frameNum);

	::sf::Vector2f getMousePosition() const;

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
	CommandCategory mCommandCategoty;
	bool mListenToInput;

	::std::map<Action, GameInput> mInputBinding;
	::std::map<Action, Command> mActionBinding;
	::std::map<uint32_t, ::std::vector<Command>> mCommandBuffer;
	::sf::RenderWindow* mWindow;
	
};
}