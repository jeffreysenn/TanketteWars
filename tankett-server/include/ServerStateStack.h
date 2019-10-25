#pragma once
#include "States/StateStack.h"
#include "ServerState.h"
#include "ServerContext.h"

class ServerStateStack : public StateStack<ServerState>
{
public:
	ServerStateStack();

	void processMessages();

};