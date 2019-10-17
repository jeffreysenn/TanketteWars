#pragma once
#include "Actor.h"
class SceneGraph :
	public Actor
{
public:
	SceneGraph();

	void enforceDestruction(Actor& actor);
};