#include "Actors/Obstacle.h"
#include "Unit.h"

namespace mw
{
Obstacle::Obstacle(float sizeX, float sizeY)
	: mCollider(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideDynamic)
{
}

Obstacle::Obstacle(float sizeX, float sizeY, const ::sf::Texture& texture)
	: SpriteActor(texture)
	, mCollider(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideDynamic)
{
	setSpriteRect(::sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	::sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	const ::sf::FloatRect colliderRect(-spriteBounds.width / 2,
									   -spriteBounds.height / 2,
									   sizeX,
									   sizeY);
	mCollider.rect = colliderRect;
}

}