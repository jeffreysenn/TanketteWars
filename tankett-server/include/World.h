#pragma once
#include "Commands/CommandQueue.h"
#include "Map.h"
#include "Actors/SceneGraph.h"
#include "Collisions/PhysicsEngine.h"


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
	::tankett::TankManager* getTankManager() { return mTankManager; }
	::mw::SceneGraph& getSceneGraph() { return mSceneGraph; }

private:
	void load();
	void buildScene(); 

private:
	CommandQueue mCommandQueue;
	::mw::PhysicsEngine mPhysicsEngine;
	::mw::SceneGraph mSceneGraph;
	::tankett::MapManager mMapManager;
	::tankett::Map* mMap;
	::tankett::TankManager* mTankManager;
};
}