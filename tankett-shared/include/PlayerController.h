#pragma once

#include "Commands/Command.h"
#include "Commands/CommandCategory.h"
#include "Input/Input.h"
#include "Network/NetRole.h"
#include "tankett_shared.h"

#include <map>
#include <vector>

namespace sf
{
class Event;
class RenderWindow;
}

namespace mw
{
class SceneGraph;
}

namespace tankett
{
class Tank;
class TankManager;
class Bullet;

class PlayerController
{
public:
	struct TankInput
	{
		bool up{}, down{}, left{}, right{}, fire{};
		float angle{};
	};

public:
	PlayerController(::mw::SceneGraph* sceneGraph = nullptr, uint8_t id = 0, bool listenToInput = false, ::sf::RenderWindow* window = nullptr, ::mw::NetRole netRole = ::mw::NetRole::Authority);
	~PlayerController();

	void handleEvent(const ::sf::Event& event, uint32_t frameNum);
	void handleRealtimeInput(uint32_t frameNum);

	::sf::Vector2f getMousePosition() const;
	
	void possessTank(Tank* tank);
	void unpossess() { mPossessedTank = nullptr; }
	Tank* getPossessedTank() const { return mPossessedTank; }

	void addBullet(Bullet* bullet) { mBullets.push_back(bullet); }
	bool removeBullet(Bullet* bullet);
	const ::std::vector<Bullet*>& getBullets() const { return mBullets; }
	::std::vector<Bullet*>& getBullets() { return mBullets; }

	uint8_t getID() { return mID; }

	void spawnTank_server(TankManager* tankManager);
	void spawnTank_client(TankManager* tankManager, ::sf::Vector2f pos);

	::std::map<uint32_t, TankInput>& getInputBuffer() { return mInputBuffer; }

	void updateTank(bool up, bool down, bool left, bool right, bool fire, float aimAngle, float deltaSeconds, uint32_t inputNum);
	void setTankState(::sf::Vector2f pos, float aimAngle);
	::tankett::PlayerState getTankState() const;

	void syncPlayerState(const tankett::PlayerState& state);
	void syncBulletState(const tankett::PlayerState& state);
	void syncTankState(const tankett::PlayerState& state);
	void lerpPlayerState(const ::tankett::PlayerState& begin, const::tankett::PlayerState& end, float t);

	void setNetRole(::mw::NetRole netRole) { mNetRole = netRole; }
	::mw::NetRole getNetRole() const { return mNetRole; }

	void setPing(uint32_t ping) { mPing = ping; }
	uint32_t getPing() const { return mPing; }

	void setScore(uint8_t score) { mScore = score; }
	void addScore(uint8_t ds) { mScore += ds; }
	uint8_t getScore() const { return mScore; }

	::sf::Vector2f getKillPos() const { return mKillPos; }
	void setKillPos(const ::sf::Vector2f& killPos) { mKillPos = killPos; }

private:
	enum class Action
	{
		Up,
		Down,
		Left,
		Right,
		Fire,
		SpawnServer,
	};

private:
	void bindInputs();

private:
	uint8_t mID;
	bool mListenToInput;

	::std::map<Action, ::mw::Input::InputCollection> mInputBinding;
	::std::map<uint32_t, TankInput> mInputBuffer;
	::sf::RenderWindow* mWindow;
	Tank* mPossessedTank;
	::std::vector<Bullet*> mBullets;
	::mw::NetRole mNetRole;
	uint32_t mPing;
	uint8_t mScore;
	mw::SceneGraph* mSceneGraph;
	::sf::Vector2f mKillPos;
};
}