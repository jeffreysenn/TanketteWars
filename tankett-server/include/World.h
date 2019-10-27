#pragma once
#include "ResourceManagers/ResourceIdentifiers.h"
#include "Commands/CommandQueue.h"
#include "Map.h"

using namespace mw;
using namespace tankett;

namespace mw
{
class Actor;
}

namespace tankett
{
class Tank;
}

namespace server
{
class World
{
public:
	World();

	void update(float deltaSeconds);

	CommandQueue& getCommandQueue() { return mCommandQueue; }

private:
	void buildScene();

private:
	Actor* mTanks;
	CommandQueue mCommandQueue;
	MapManager mMapManager;
};

}