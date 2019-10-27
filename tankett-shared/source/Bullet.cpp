#include "Bullet.h"
#include "Rendering/Renderer.h"
#include "Units/unit.h"
#include "Tank.h"

namespace tankett
{
Bullet::Bullet(Tank* owner)
	: mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mOwner(owner)
{
}

Bullet::Bullet(const ::sf::Texture& texture, Tank* owner)
	: SpriteActor(texture, Rendering::Bullet)
	, mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mOwner(owner)
{
	::sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	getSprite()->setOrigin(spriteBounds.width / 2, spriteBounds.height);
	getSprite()->setRotation(90);
	const ::sf::FloatRect colliderRect(0, -spriteBounds.width / 2, unit::unit2pix(0.2f), unit::unit2pix(0.2f));
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

	Bullet* bullet = dynamic_cast<Bullet*>(&other);
	if (bullet)
		return;

	destroy(this);
}

Collider* Bullet::getCollider()
{
	return &mCollider;
}
}
