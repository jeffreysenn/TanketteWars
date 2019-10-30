#pragma once

#include "Commands/Command.h"
#include "Commands/CommandCategory.h"
#include "Input/Input.h"
#include "Network/NetRole.h"

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
	PlayerController(uint8_t id = 0, bool listenToInput = false, ::sf::RenderWindow* window = nullptr, ::mw::NetRole netRole = NetRole::Authority);

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

	void updateTank(bool up, bool down, bool left, bool right, bool fire, float aimAngle, float deltaSeconds);
	void setTankState(::sf::Vector2f pos, float aimAngle);

	void setNetRole(::mw::NetRole netRole) { mNetRole = netRole; }
	::mw::NetRole getNetRole() const { return mNetRole; }

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
	Command mMousePosCommand;
	bool mListenToInput;

	::std::map<Action, ::mw::Input::InputCollection> mInputBinding;
	::std::map<Action, Command> mInputActionBinding;
	::std::map<Action, Command> mCommands;
	::std::map<uint32_t, TankInput> mInputBuffer;
	::sf::RenderWindow* mWindow;
	Tank* mPossessedTank;
	::std::vector<Bullet*> mBullets;
	NetRole mNetRole;
};
}