#include "SceneGraph.h"



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
			children[i]->getParent()->detachChild(*children[i]);
		else
			enforceDestruction(*children[i]);
	}
}

