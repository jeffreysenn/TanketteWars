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
class Tank;
class TankManager;
class PlayerController
{
public:
	struct TankInput
	{
		void setDirections(bool up, bool down, bool left, bool right);
		void setFire(bool fire);

		uint8_t buttom;
		float angle;
	};

public:
	PlayerController(uint8_t id = 0, bool listenToInput = false, ::sf::RenderWindow* window = nullptr);

	void handleEvent(const ::sf::Event& event, uint32_t frameNum);
	void handleRealtimeInput(uint32_t frameNum);

	::sf::Vector2f getMousePosition() const;
	
	void possessTank(Tank* tank);
	void unpossess() { mPossessedTank = nullptr; }

	uint8_t getID() { return mID; }

	void spawnTank_server(TankManager* tankManager);
	void spawnTank_client(TankManager* tankManager, ::sf::Vector2f pos);

	float getTankTurretAngle();
	::sf::Vector2f getTankPosition() const;

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
};
}