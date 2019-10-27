#pragma once
#include "States/StateStack.h"
#include "ServerState.h"
#include "Context.h"
namespace server
{

class ServerStateStack : public mw::StateStack<ServerState>
{
public:
	ServerStateStack();

	void processMessages();

};
}