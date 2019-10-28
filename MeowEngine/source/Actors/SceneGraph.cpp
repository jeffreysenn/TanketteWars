#include "Actors/SceneGraph.h"
#include "Actors/CameraActor.h"

namespace mw
{

SceneGraph::SceneGraph()
	: Actor()
{
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