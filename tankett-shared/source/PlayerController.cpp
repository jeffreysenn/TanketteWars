#include "PlayerController.h"
#include "Tank.h"
#include "TankManager.h"
#include "Commands/CommandQueue.h"
#include "Actors/SceneGraph.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "tankett_debug.h"

namespace tankett
{
PlayerController::PlayerController(uint8_t id, bool listenToInput, ::sf::RenderWindow* window, ::mw::NetRole netRole)
	: mWindow(window)
	, mListenToInput(listenToInput)
	, mPossessedTank(nullptr)
	, mID(id)
	, mNetRole(netRole)
	, mBullets{}
	, mPing(~0u)
	, mScore(0u)
{
	if (mListenToInput)
	{
		bindInputs();
	}
}

void PlayerController::handleEvent(const ::sf::Event& event, uint32_t frameNum)
{
}

constexpr size_t bufferSize = 120;
constexpr size_t cleanBufferThreshold = 600;
void PlayerController::handleRealtimeInput(uint32_t frameNum)
{
	if (!(mListenToInput && mPossessedTank))
		return;

	bool up{}, down{}, left{}, right{}, fire{};
	if (::mw::Input::inputCollectionPressed(mInputBinding[Action::Up]))
	{
		up = true;
	}
	if (::mw::Input::inputCollectionPressed(mInputBinding[Action::Down]))
	{
		down = true;
	}
	if (::mw::Input::inputCollectionPressed(mInputBinding[Action::Left]))
	{
		left = true;
	}
	if (::mw::Input::inputCollectionPressed(mInputBinding[Action::Right]))
	{
		right = true;
	}
	mPossessedTank->addDirection((float)(-(int)left + (int)right), (float)(-(int)up + (int)down));

	float aimAngle = mPossessedTank->mousePosToAngle(getMousePosition());
	mPossessedTank->aimAt(aimAngle);

	if (::mw::Input::inputCollectionPressed(mInputBinding[Action::Fire]))
	{
		mPossessedTank->fire(frameNum);
		fire = true;
	}


	mInputBuffer[frameNum].up = up;
	mInputBuffer[frameNum].down = down;
	mInputBuffer[frameNum].left = left;
	mInputBuffer[frameNum].right = right;
	mInputBuffer[frameNum].fire = fire;
	mInputBuffer[frameNum].angle = aimAngle;


	// discard the old buffers
	auto it = mInputBuffer.find(frameNum - cleanBufferThreshold);
	if (it != mInputBuffer.end())
	{
		auto eraseTill = mInputBuffer.find(frameNum - bufferSize);
		mInputBuffer.erase(mInputBuffer.begin(), eraseTill);
	}
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

bool PlayerController::removeBullet(Bullet* bullet)
{
	for (auto it = mBullets.begin(); it != mBullets.end(); ++it)
	{
		if (*it == bullet)
		{
			mBullets.erase(it);
			return true;
		}
	}
	return false;
}

void PlayerController::spawnTank_server(TankManager* tankManager)
{
	tankManager->spawnTankRandom(mID, this, mNetRole);
}

void PlayerController::spawnTank_client(TankManager* tankManager, ::sf::Vector2f pos)
{
	tankManager->spawnTank(pos, mID, this, mNetRole);
}

void PlayerController::updateTank(bool up, bool down, bool left, bool right, bool fire, float aimAngle, float deltaSeconds, uint32_t inputNum)
{
	if (!mPossessedTank) return;
	mPossessedTank->addDirection((float)(-(int)left + (int)right), (float)(-(int)up + (int)down));
	mPossessedTank->aimAt(aimAngle);
	if (fire)
		mPossessedTank->fire(inputNum);

	::mw::SceneGraph* sceneGraph = (::mw::SceneGraph*) mPossessedTank->getSceneGraph();
	if (sceneGraph)
	{
		sceneGraph->update(deltaSeconds);
		sceneGraph->checkSceneCollision();
		sceneGraph->enforceDestruction(*sceneGraph);
	}
}

void PlayerController::setTankState(::sf::Vector2f pos, float aimAngle)
{
	if (!mPossessedTank) return;
	mPossessedTank->setPosition(pos);
	mPossessedTank->aimAt(aimAngle);
}

::tankett::PlayerState PlayerController::getTankState() const
{
	PlayerState state;
	state.client_id = mID;
	if (!mPossessedTank)
	{
		state.alive = false;
	}
	else
	{
		state.alive = true;
		state.angle = mPossessedTank->getTurretAngle();
		state.position = ::alpha::vector2(mPossessedTank->getPosition().x, mPossessedTank->getPosition().y);
	}

	state.eliminations = mScore;
	state.bullet_count = (uint8_t) mBullets.size();
	for(int i = 0; i < mBullets.size(); ++i)
	{
		for (const auto& bullet : mBullets)
		{
			state.bullets[i].id = bullet->getID();
			state.bullets[i].position = ::alpha::vector2(bullet->getPosition().x, bullet->getPosition().y);
		}
	}

	return state;
}

void PlayerController::bindInputs()
{
	mInputBinding[Action::Up] =
	{
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::W},
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::Up}
	};

	mInputBinding[Action::Down] =
	{
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::S},
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::Down}
	};

	mInputBinding[Action::Left] =
	{
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::A},
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::Left}
	};

	mInputBinding[Action::Right] =
	{
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::D},
		{::mw::Input::Type::Keyboard, ::sf::Keyboard::Right}
	};

	mInputBinding[Action::Fire] =
	{
		{::mw::Input::Type::Mouse, ::sf::Mouse::Left},
		{ ::mw::Input::Type::Keyboard, ::sf::Keyboard::Space }
	};
}
}