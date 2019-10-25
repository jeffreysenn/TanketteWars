#pragma once
#include "ServerState.h"

class WaitingState : public ServerState
{
public:
	WaitingState();

	void processMessages() override;
};
