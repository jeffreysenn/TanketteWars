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
	auto it = mInputBuffer.find(frameNum - bufferSize);
	if (it != mInputBuffer.end())
	{
		mInputBuffer.erase(mInputBuffer.begin(), it);
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

	mPossessedTank->update(deltaSeconds);
	for (auto& bullet : mBullets)
	{
		bullet->update(deltaSeconds);
	}

	::mw::SceneGraph* sceneGraph = (::mw::SceneGraph*) mPossessedTank->getSceneGraph();
	if (sceneGraph)
	{
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

void PlayerController::syncPlayerState(const tankett::PlayerState& state)
{
	syncBulletState(state);
	syncTankState(state);
}

void PlayerController::syncBulletState(const tankett::PlayerState& state)
{
	// sync the bullets
	// bullets exist both on server and local: transform
	// bullets exist only on local: destroy
	// bullets exist only on server: spawn
	uint8_t bulletCount = state.bullet_count;
	const auto& bullets = state.bullets;
	auto& existingBullets = getBullets();
	::std::map<uint8_t, ::std::pair<::tankett::Bullet*, const ::tankett::bullet_data*>> bulletMap{};
	for (int bulletIndex = 0; bulletIndex < bulletCount; ++bulletIndex)
	{
		bulletMap[bullets[bulletIndex].id].second = &bullets[bulletIndex];
	}
	for (auto& existingBullet : existingBullets)
	{
		bulletMap[existingBullet->getID()].first = existingBullet;
	}
	for (auto& it : bulletMap)
	{
		auto& bulletPair = it.second;

		if (bulletPair.first && bulletPair.second)
		{
			bulletPair.first->setPosition(bulletPair.second->position.x_, bulletPair.second->position.y_);
		}
		else if (bulletPair.first && !bulletPair.second)
		{
			::mw::Actor::destroy(bulletPair.first);
		}
		else if (!bulletPair.first && bulletPair.second)
		{
			if (mPossessedTank)
			{
				::tankett::Bullet* newBullet = mPossessedTank->spawnBullet(mPossessedTank->getTurretAngle());
				newBullet->setID(bulletPair.second->id);
				newBullet->setPosition(bulletPair.second->position.x_, bulletPair.second->position.y_);
			}
		}
	}
}

void PlayerController::syncTankState(const tankett::PlayerState& state)
{
	if (state.alive)
	{
		::sf::Vector2f pos = ::sf::Vector2f(state.position.x_, state.position.y_);
		float aimAngle = state.angle;
		setTankState(pos, aimAngle);
	}
	else if (mPossessedTank)
	{
		mPossessedTank->destroy(mPossessedTank);
		::mw::SceneGraph* sceneGraph = (::mw::SceneGraph*) mPossessedTank->getSceneGraph();
		sceneGraph->enforceDestruction(*sceneGraph);
	}
}

void PlayerController::lerpPlayerState(const ::tankett::PlayerState& begin, const::tankett::PlayerState& end, float t)
{
	::tankett::PlayerState currentState;

	currentState.alive = end.alive;
	float deltaAngle = end.angle - begin.angle;
	currentState.angle = begin.angle + deltaAngle * t;
	currentState.position.x_ = begin.position.x_ + (end.position.x_ - begin.position.x_) * t;
	currentState.position.y_ = begin.position.y_ + (end.position.y_ - begin.position.y_) * t;
	currentState.bullet_count = end.bullet_count;

	// lerp the existing bullets
	// use end state's bullet states for new bullets
	// exclude bullets which do not exist in the end state
	for (int endBulletIndex = 0; endBulletIndex < end.bullet_count; ++endBulletIndex)
	{
		currentState.bullets[endBulletIndex].id = end.bullets[endBulletIndex].id;

		int foundBeginBulletIndex = -1;
		for (int beginBulletIndex = 0; beginBulletIndex < begin.bullet_count; ++beginBulletIndex)
		{
			if (end.bullets[endBulletIndex].id == begin.bullets[beginBulletIndex].id)
			{
				foundBeginBulletIndex = beginBulletIndex;
			}
		}

		// lerp
		if (foundBeginBulletIndex != -1)
		{
			currentState.bullets[endBulletIndex].position.x_ = end.bullets[endBulletIndex].position.x_ + (end.bullets[endBulletIndex].position.x_ - begin.bullets[foundBeginBulletIndex].position.x_) * t;
			currentState.bullets[endBulletIndex].position.y_ = end.bullets[endBulletIndex].position.y_ + (end.bullets[endBulletIndex].position.y_ - begin.bullets[foundBeginBulletIndex].position.y_) * t;
		}
		else
		{
			currentState.bullets[endBulletIndex].position = end.bullets[endBulletIndex].position;
		}
	}

	syncPlayerState(currentState);
	if (mPossessedTank)
	{
		::sf::Vector2f direction(currentState.position.x_ - begin.position.x_, currentState.position.y_ - begin.position.y_);
		mPossessedTank->setDirection(direction); 
	}
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