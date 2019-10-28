#include "PlayerController.h"
#include "Tank.h"
#include "TankManager.h"
#include "Commands/CommandQueue.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace tankett
{
PlayerController::PlayerController(CommandCategory category, bool listenToInput, ::sf::RenderWindow* window)
	: mWindow(window)
	, mCommandCategoty(category)
	, mListenToInput(listenToInput)
	, mPossessedTank(nullptr)
{
	if (mListenToInput)
	{
		bindInputs();
		bindInputActions();
	}
	bindCommands();
}

void PlayerController::handleEvent(const ::sf::Event& event, CommandQueue& commandQueue, uint32_t frameNum)
{
	if (!mListenToInput)
		return;

	for (auto& pair : mInputActionBinding)
	{
		const auto& gameInput = mInputBinding[pair.first];
		if ((!gameInput.bIsRealTime) &&
			Input::eventInputCollectionPressed(event, gameInput.inputCollection))
		{
			commandQueue.push(pair.second);
			mCommandBuffer[frameNum].push_back(pair.second);
		}
	}
}

constexpr size_t bufferSize = 60;
void PlayerController::handleRealtimeInput(CommandQueue& commandQueue, uint32_t frameNum)
{
	if (!mListenToInput)
		return;

	for (auto& pair : mInputActionBinding)
	{
		const auto& gameInput = mInputBinding[pair.first];
		if (gameInput.bIsRealTime &&
			Input::inputCollectionPressed(gameInput.inputCollection))
		{
			commandQueue.push(pair.second);
			mCommandBuffer[frameNum].push_back(pair.second);

			// discard the old buffers
			auto it = mCommandBuffer.find(frameNum - bufferSize);
			if (it != mCommandBuffer.end())
				mCommandBuffer.erase(mCommandBuffer.begin(), it);
		}
	}

	commandQueue.push(mMousePosCommand);
}

::sf::Vector2f PlayerController::getMousePosition() const
{
	if (mWindow)
		return mWindow->mapPixelToCoords(::sf::Mouse::getPosition(*mWindow));

	return ::sf::Vector2f();
}

void PlayerController::spawnTankServer(CommandQueue& commandQueue, uint32_t frameNum)
{
	commandQueue.push(mCommands[Action::SpawnServer]);
}

void PlayerController::spawnTankClient(CommandQueue& commandQueue, uint32_t frameNum, const ::sf::Vector2f& pos)
{
	::mw::Command command;
	command.action =
		derivedAction<TankManager>([&](TankManager& tankManager, float)
								   {
									   tankManager.spawnTank(pos, mCommandCategoty, mListenToInput, this);
								   });
	command.category = CommandCategory::TankManager;
	commandQueue.push(command);
}

void PlayerController::bindInputs()
{
	mInputBinding[Action::Up] =
	{
		{
			{Input::Type::Keyboard, ::sf::Keyboard::W},
			{Input::Type::Keyboard, ::sf::Keyboard::Up}
		}, true
	};

	mInputBinding[Action::Down] =
	{
		{
			{Input::Type::Keyboard, ::sf::Keyboard::S},
			{Input::Type::Keyboard, ::sf::Keyboard::Down}
		}, true
	};

	mInputBinding[Action::Left] =
	{
		{
			{Input::Type::Keyboard, ::sf::Keyboard::A},
			{Input::Type::Keyboard, ::sf::Keyboard::Left}
		}, true
	};

	mInputBinding[Action::Right] =
	{
		{
			{Input::Type::Keyboard, ::sf::Keyboard::D},
			{Input::Type::Keyboard, ::sf::Keyboard::Right}
		}, true
	};

	mInputBinding[Action::Fire] =
	{
		{{Input::Type::Mouse, ::sf::Mouse::Left}},
		false
	};
}

void PlayerController::bindInputActions()
{
	mInputActionBinding[Action::Up].action =
		derivedAction<Tank>([&](Tank& tank, float) { tank.addDirection(0, -1); });

	mInputActionBinding[Action::Down].action =
		derivedAction<Tank>([&](Tank& tank, float) { tank.addDirection(0, 1); });

	mInputActionBinding[Action::Left].action =
		derivedAction<Tank>([&](Tank& tank, float) { tank.addDirection(-1, 0); });

	mInputActionBinding[Action::Right].action =
		derivedAction<Tank>([&](Tank& tank, float) { tank.addDirection(1, 0); });

	mInputActionBinding[Action::Fire].action =
		derivedAction<Tank>([&](Tank& tank, float) { tank.fire(); });

	for (auto& pair : mInputActionBinding)
		pair.second.category = mCommandCategoty;

	mMousePosCommand.action =
		derivedAction<Tank>([&](Tank& tank, float)
							{
								tank.aimAt(getMousePosition());
							});

	mMousePosCommand.category = mCommandCategoty;
}

void PlayerController::bindCommands()
{
	mCommands[Action::SpawnServer].action =
		derivedAction<TankManager>([&](TankManager& tankManager, float)
								   {
									   tankManager.spawnTankRandom(mCommandCategoty, mListenToInput, this);
								   });
	mCommands[Action::SpawnServer].category = CommandCategory::TankManager;
	
	
}
}