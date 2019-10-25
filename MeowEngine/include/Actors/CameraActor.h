#pragma once
#include <SFML/Graphics/View.hpp>
#include "Actors/MovableActor.h"

namespace mw
{
	class CameraActor : public ::sf::View, public MovableActor
	{
	public:
		CameraActor();
		~CameraActor();

		virtual void updateSelf(float deltaSeconds) override;
	};
}