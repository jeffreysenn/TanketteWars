#pragma once
namespace server
{

class ServerState
{
public:
	ServerState();

	virtual void processMessages();

	virtual void packMessages();

};


}