#pragma once
#include "ServerState.h"
#include "PlayerController.h"
#include "tankett_shared.h"
#include "World.h"
#include <SFML/System/Clock.hpp>
#include <map>

namespace server
{
class NetworkManager;

class GameState : public ServerState
{
public:
	GameState();

	void processMessages() override;
	void checkRespawn();
	void packMessages() override;

private:
	void checkJoin();
	void checkQuit();
	void applyInput();
	void checkTime();

private:
	World mWorld;
	::std::map<uint8_t, ::tankett::PlayerController> mControllers;
	::std::map<uint8_t, bool> mRespawnMap;
	::std::map<uint8_t, ::sf::Clock> mRespawnClocks;
	NetworkManager& mNetworkManager;
	::sf::Clock mRoundClock;
};
}