#pragma once
#include "Game.h"
#include "ServerNetworkManager.h"
#include "ServerStateStack.h"
#include "ServerContext.h"

using namespace tankett;

class TanketteWarServer : public mw::Game
{
public:
	TanketteWarServer();

	void run() override;

private:
	void init();

	void registerStates();

private:
	ServerNetworkManager mNetworkManager;
	ServerStateStack mStateStack;
};
