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
				auto& destroyingChildren = pendingDestroy->getChildren();
				for (auto& destroyingChild : destroyingChildren)
				{
					CameraActor* camera = dynamic_cast<CameraActor*>(destroyingChild.get());
					if (camera)
						attachChild(::std::move(destroyingChild));
				}

			}
			else
				enforceDestruction(*children[i]);
		}
	}


}