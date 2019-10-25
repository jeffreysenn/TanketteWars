#pragma once
#include "SpriteActor.h"
#include "../Collisions/Collider.h"

class Obstacle : public SpriteActor
{
public:
	Obstacle(float sizeX, float sizeY, const sf::Texture& texture);

	virtual Collider* getCollider() override { return &mCollider; }

private:
	Collider mCollider;
};