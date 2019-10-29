#pragma once
#include "Actor.h"
namespace mw
{
class PhysicsEngine;

class SceneGraph :
	public Actor
{
public:
	SceneGraph(PhysicsEngine* physicsEngine = nullptr);

	void checkSceneCollision();

	void enforceDestruction(Actor& actor);

private:
	PhysicsEngine* mPhysicsEngine;
};
}