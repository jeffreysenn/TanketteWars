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

private:
	void registerCollider(float sizeX, float sizeY);
};
}