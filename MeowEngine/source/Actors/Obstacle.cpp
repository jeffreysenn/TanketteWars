#include "Actors/Obstacle.h"
#include "Unit.h"

namespace mw
{
Obstacle::Obstacle(float sizeX, float sizeY)
	: mCollider(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideDynamic)
{
	setSpriteRect(::sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	const ::sf::FloatRect colliderRect(-sizeX / 2,
									   -sizeY / 2,
									   sizeX,
									   sizeY);
	mCollider.rect = colliderRect;
}

Obstacle::Obstacle(float sizeX, float sizeY, const ::sf::Texture& texture)
	: SpriteActor(texture)
	, mCollider(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideDynamic)
{
	setSpriteRect(::sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	const ::sf::FloatRect colliderRect(-sizeX / 2,
									   -sizeY / 2,
									   sizeX,
									   sizeY);
	mCollider.rect = colliderRect;
}

}