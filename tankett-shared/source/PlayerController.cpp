#include "PlayerController.h"
#include "Tank.h"
#include "TankManager.h"
#include "Commands/CommandQueue.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace tankett
{
PlayerController::PlayerController(uint8_t id, bool listenToInput, ::sf::RenderWindow* window)
	: mWindow(window)
	, mListenToInput(listenToInput)
	, mPossessedTank(nullptr)
	, mID(id)
{
	if (mListenToInput)
	{
		bindInputs();
	}
}

void PlayerController::handleEvent(const ::sf::Event& event, uint32_t frameNum)
{
	if (!(mListenToInput && mPossessedTank))
		return;

	if (Input::eventInputCollectionPressed(event, mInputBinding[Action::Fire]))
	{
		mPossessedTank->fire();
		mInputBuffer[frameNum].setFire(true);
	}

}

constexpr size_t bufferSize = 60;
void PlayerController::handleRealtimeInput(uint32_t frameNum)
{
	if (!(mListenToInput && mPossessedTank))
		return;

	bool up{}, down{}, left{}, right{};
	if (Input::inputCollectionPressed(mInputBinding[Action::Up]))
	{
		mPossessedTank->addDirection(0, -1);
		up = true;
	}
	if (Input::inputCollectionPressed(mInputBinding[Action::Down]))
	{
		mPossessedTank->addDirection(0, 1);
		down = true;
	}
	if (Input::inputCollectionPressed(mInputBinding[Action::Left]))
	{
		mPossessedTank->addDirection(-1, 0);
		left = true;
	}
	if (Input::inputCollectionPressed(mInputBinding[Action::Right]))
	{
		mPossessedTank->addDirection(1, 0);
		right = true;
	}

	float aimAngle = mPossessedTank->mousePosToAngle(getMousePosition());
	mPossessedTank->aimAt(aimAngle);

	mInputBuffer[frameNum].setDirections(up, down, left, right);
	mInputBuffer[frameNum].angle = aimAngle;

	// discard the old buffers
	auto it = mInputBuffer.find(frameNum - bufferSize);
	if (it != mInputBuffer.end())
		mInputBuffer.erase(mInputBuffer.begin(), it);
}

::sf::Vector2f PlayerController::getMousePosition() const
{
	if (mWindow)
		return mWindow->mapPixelToCoords(::sf::Mouse::getPosition(*mWindow));

	return ::sf::Vector2f();
}

void PlayerController::possessTank(Tank* tank)
{
	mPossessedTank = tank; 
	tank->setController(this);
}

void PlayerController::spawnTank_server(TankManager* tankManager)
{
	tankManager->spawnTankRandom(mID, this, mListenToInput);
}

void PlayerController::spawnTank_client(TankManager* tankManager, ::sf::Vector2f pos)
{
	tankManager->spawnTank(pos, mID, this, mListenToInput);
}

float PlayerController::getTankTurretAngle()
{
	if (mPossessedTank)
	{
		return mPossessedTank->getTurretAngle();
	}
	return 0.0f;
}

::sf::Vector2f PlayerController::getTankPosition() const
{
	if (mPossessedTank)
	{
		return mPossessedTank->getWorldPosition();
	}
	return ::sf::Vector2f();
}

void PlayerController::bindInputs()
{
	mInputBinding[Action::Up] =
	{
			{Input::Type::Keyboard, ::sf::Keyboard::W},
			{Input::Type::Keyboard, ::sf::Keyboard::Up}
	};

	mInputBinding[Action::Down] =
	{
			{Input::Type::Keyboard, ::sf::Keyboard::S},
			{Input::Type::Keyboard, ::sf::Keyboard::Down}
	};

	mInputBinding[Action::Left] =
	{
			{Input::Type::Keyboard, ::sf::Keyboard::A},
			{Input::Type::Keyboard, ::sf::Keyboard::Left}
	};

	mInputBinding[Action::Right] =
	{
		{Input::Type::Keyboard, ::sf::Keyboard::D},
		{Input::Type::Keyboard, ::sf::Keyboard::Right}
	};

	mInputBinding[Action::Fire] =
	{
		{Input::Type::Mouse, ::sf::Mouse::Left}
	};
}
void PlayerController::TankInput::setDirections(bool up, bool down, bool left, bool right)
{
	buttom |= up << 4;
	buttom |= down << 3;
	buttom |= left << 2;
	buttom |= right << 1;
}
void PlayerController::TankInput::setFire(bool fire)
{
	buttom |= fire << 0;
}
}