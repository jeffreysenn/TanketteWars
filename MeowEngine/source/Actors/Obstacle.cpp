#include "Actors/Obstacle.h"
#include "Unit.h"

namespace mw
{
Obstacle::Obstacle(float sizeX, float sizeY)
{
	setSpriteRect(::sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	registerCollider(sizeX, sizeY);
}

Obstacle::Obstacle(float sizeX, float sizeY, const ::sf::Texture& texture)
	: SpriteActor(texture)
{
	setSpriteRect(::sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	registerCollider(sizeX, sizeY);
}

void Obstacle::registerCollider(float sizeX, float sizeY)
{
	const ::sf::FloatRect colliderRect(-sizeX / 2, -sizeY / 2, sizeX, sizeY);
	auto collider = ::std::make_unique<Collider>(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideDynamic, colliderRect);
	setCollider(collider);
}
}