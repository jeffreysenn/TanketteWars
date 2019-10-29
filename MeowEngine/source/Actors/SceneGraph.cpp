#include "Actors/SceneGraph.h"
#include "Actors/CameraActor.h"
#include "Collisions/PhysicsEngine.h"

namespace mw
{

SceneGraph::SceneGraph(PhysicsEngine* physicsEngine)
	: Actor()
	, mPhysicsEngine(physicsEngine)
{
}

void SceneGraph::checkSceneCollision()
{
	if (!mPhysicsEngine) return;

	reportCollisionInfo(*mPhysicsEngine);
	mPhysicsEngine->checkCollision();
}

void SceneGraph::enforceDestruction(Actor& actor)
{
	auto& children = actor.getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i]->isPendingDestroy())
		{
			auto pendingDestroy = children[i]->getParent()->detachChild(*children[i]);
		}
		else
		{
			enforceDestruction(*children[i]);
		}
	}
}

}