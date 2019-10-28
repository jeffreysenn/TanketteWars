#pragma once
#include "ServerState.h"
#include "PlayerController.h"
#include "tankett_shared.h"
#include "World.h"
#include <map>

namespace server
{
class NetworkManager;

class GameState : public ServerState
{
public:
	GameState();

	void processMessages() override;

private:
	void checkJoin();

private:
	World mWorld;
	::std::map<::alpha::ip_address, ::tankett::PlayerController> mControllers;
	NetworkManager& mNetworkManager;

};
}