#pragma once
#include "Game.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
#include "EliminationHeatMap.h"
#include "Context.h"

using namespace tankett;
namespace server
{

class TanketteWarServer : public mw::Game
{
public:
	TanketteWarServer();

	void run() override;

private:
	void init();

	void registerStates();

private:
	NetworkManager mNetworkManager;
	ServerStateStack mStateStack;
	HeatMapManager mHeatMapManager;
};

}