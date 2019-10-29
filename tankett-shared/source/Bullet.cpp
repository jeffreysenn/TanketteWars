#include "Bullet.h"
#include "Rendering/Renderer.h"
#include "Unit.h"
#include "Tank.h"

namespace tankett
{
Bullet::Bullet(Tank* owner)
	: mOwner(owner)
	, mID(0)
{
	owner->addBullet(this);
	registerCollider();
}

Bullet::Bullet(const ::sf::Texture& texture, Tank* owner)
	: SpriteActor(texture, Rendering::Bullet)
	, mOwner(owner)
	, mID(0)
{
	owner->addBullet(this);

	::sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	getSprite()->setOrigin(spriteBounds.width / 2, spriteBounds.height);
	getSprite()->setRotation(90);

	registerCollider();
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
void Bullet::registerCollider()
{
	float width = unit::unit2pix(0.2f);
	float height = unit::unit2pix(0.2f);
	const ::sf::FloatRect colliderRect(0, -width / 2, width, height);
	auto collider = ::std::make_unique<Collider>(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll, colliderRect);
	setCollider(collider);
}
}
