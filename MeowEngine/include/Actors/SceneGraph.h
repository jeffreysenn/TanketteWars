#pragma once
#include "Actor.h"
namespace mw
{
	class SceneGraph :
		public Actor
	{
	public:
		SceneGraph();

		void enforceDestruction(Actor& actor);
	};
}