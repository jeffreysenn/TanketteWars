#include "World.h"
#include "TankManager.h"
#include "Map.h"
#include "Unit.h"

namespace server
{
World::World()
	: mMap(nullptr)
	, mTankManager(nullptr)
	, mSceneGraph(&mPhysicsEngine)
{
	load();
	buildScene();
}

void World::executeCommands(float deltaSeconds)
{
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), deltaSeconds);
}


void World::load()
{
	mMapManager.load(::tankett::Map::DefaultMap, "../Assets/Maps/DefaultMap.txt");
}

void World::buildScene()
{
	mMap = mMapManager.get(::tankett::Map::DefaultMap);
	auto mapObj = mMap->buildMap(::tankett::unit::unit2pix(1), ::tankett::unit::unit2pix(1));
	mSceneGraph.attachChild(::std::move(mapObj));

	auto tankManager = ::std::make_unique<::tankett::TankManager>(mMap);
	mTankManager = tankManager.get();
	mSceneGraph.attachChild(::std::move(tankManager));
}
}