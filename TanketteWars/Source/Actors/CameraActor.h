#pragma once
#include <SFML/Graphics/View.hpp>
#include "MovableActor.h"

class CameraActor : public sf::View, public MovableActor
{
public:
	CameraActor();
	~CameraActor();

	virtual void updateSelf(float deltaSeconds) override;
};