#pragma once
#include "ServerState.h"
namespace server
{

class WaitingState : public ServerState
{
public:
	WaitingState();

	void processMessages() override;
};

}