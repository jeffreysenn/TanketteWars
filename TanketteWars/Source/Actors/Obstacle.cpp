#include "Obstacle.h"
#include "../Units/Units.h"

Obstacle::Obstacle(float sizeX, float sizeY, const sf::Texture& texture)
	: SpriteActor(texture)
	, mCollider(Collision::ObjectType::Static, Collision::ObjectResponsePreset::CollideAll)
{
	setSpriteRect(sf::IntRect(0, 0, (int)sizeX, (int)sizeY));
	sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	const sf::FloatRect colliderRect(-spriteBounds.width / 2,
									 -spriteBounds.height / 2,
									 Units::unit2pix(1),
									 Units::unit2pix(1));
	mCollider.rect = colliderRect;
}
