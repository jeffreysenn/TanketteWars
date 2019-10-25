#include "Controllers/PlayerController.h"
#include "Actors/Tank.h"
#include "Commands/CommandQueue.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace mw
{
	void upAction(Tank& tank, float)
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

	::sf::Vector2f mousePos;
	void mousePosAction(Tank& tank, float)
	{
		tank.aimAt(mousePos);
	}

	void fireAction(Tank& tank, float)
	{
		tank.fire();
	}

	PlayerController::PlayerController()
		: mWindow(nullptr)
	{
		bindInputs();
		bindActions();
	}

	PlayerController::PlayerController(::sf::RenderWindow& window)
		: mWindow(&window)
	{
		bindInputs();
		bindActions();
	}

	void PlayerController::handleEvent(const ::sf::Event& event, CommandQueue& commandQueue, uint32_t frameNum)
	{
		for (auto& pair : mActionBinding)
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
		for (auto& pair : mActionBinding)
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

		mousePos = getMousePosition();
		commandQueue.push(mMousePosCommand);
	}

	::sf::Vector2f PlayerController::getMousePosition() const
	{
		if (mWindow)
			return mWindow->mapPixelToCoords(::sf::Mouse::getPosition(*mWindow));

		return ::sf::Vector2f();
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

		for (auto& pair : mActionBinding)
			pair.second.category = CommandCategory::Tank0;

		mMousePosCommand.action = derivedAction<Tank>(mousePosAction);
		mMousePosCommand.category = CommandCategory::Tank0;
	}


}