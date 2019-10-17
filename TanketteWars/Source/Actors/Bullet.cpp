#include "Bullet.h"
#include "../Rendering/Renderer.h"
#include "../Units/Units.h"
#include "Tank.h"


Bullet::Bullet(const sf::Texture& texture, Tank* owner)
	: SpriteActor(texture, Rendering::Bullet)
	, mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mOwner(owner)
{
	sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	getSprite()->setOrigin(spriteBounds.width / 2, spriteBounds.height);
	getSprite()->setRotation(90);
	const sf::FloatRect colliderRect(0, -spriteBounds.width/2, Units::unit2pix(0.2f), Units::unit2pix(0.2f));
	mCollider.rect = colliderRect;
}

Bullet::~Bullet()
{
	if (mOwner)
		mOwner->removeBullet(this);
}

void Bullet::onCollisionEnter(Actor& other)
{
	if (&other == mOwner)
		return;

	destroy(this);
}

Collider* Bullet::getCollider()
{
	return &mCollider;
}
