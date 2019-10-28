#pragma once
#include "Commands/CommandQueue.h"
#include "Map.h"
#include "Actors/SceneGraph.h"

namespace mw
{
class Actor;
}

namespace tankett
{
class Tank;
class TankManager;
}

namespace server
{
class World
{
public:
	World();

	void executeCommands(float deltaSeconds);

	CommandQueue& getCommandQueue() { return mCommandQueue; }

private:
	void load();
	void buildScene(); 

private:
	CommandQueue mCommandQueue;
	::mw::SceneGraph mSceneGraph;
	::tankett::MapManager mMapManager;
	::tankett::Map* mMap;
	::tankett::TankManager* mTankManager;
};
}