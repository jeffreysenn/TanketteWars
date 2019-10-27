#pragma once
#include "SpriteActor.h"
#include "Collisions/Collider.h"
namespace mw
{
class Obstacle : public SpriteActor
{
public:
	Obstacle(float sizeX, float sizeY);
	Obstacle(float sizeX, float sizeY, const ::sf::Texture& texture);

	virtual Collider* getCollider() override { return &mCollider; }

private:
	Collider mCollider;
};
}