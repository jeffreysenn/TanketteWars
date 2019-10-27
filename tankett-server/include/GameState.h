#pragma once
#include "ServerState.h"
namespace server
{

class GameState : public ServerState
{
public:
	GameState();

	void processMessages() override;
};


}