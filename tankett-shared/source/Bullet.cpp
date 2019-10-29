#include "Bullet.h"
#include "Rendering/Renderer.h"
#include "Unit.h"
#include "Tank.h"

namespace tankett
{
Bullet::Bullet(Tank* owner)
	: mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mOwner(owner)
	, mID((uint8_t)owner->getBullets().size())
{
	owner->addBullet(this);

	float width = unit::unit2pix(0.2f);
	float height = unit::unit2pix(0.2f);
	const ::sf::FloatRect colliderRect(0, -width / 2, width, height);
	mCollider.rect = colliderRect;
}

Bullet::Bullet(const ::sf::Texture& texture, Tank* owner)
	: SpriteActor(texture, Rendering::Bullet)
	, mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mOwner(owner)
	, mID((uint8_t)owner->getBullets().size())
{
	owner->addBullet(this);

	::sf::FloatRect spriteBounds(getSprite()->getLocalBounds());
	getSprite()->setOrigin(spriteBounds.width / 2, spriteBounds.height);
	getSprite()->setRotation(90);
	float width = unit::unit2pix(0.2f);
	float height = unit::unit2pix(0.2f);
	const ::sf::FloatRect colliderRect(0, -width / 2, width, height);
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

void Bullet::setIsLocal(bool isLocal)
{
	mIsLocal = isLocal;
	if (!isLocal)
	{
		mCollider.response = ::mw::Collision::ObjectResponse(Collision::ObjectResponsePreset::NoCollision);
	}
	else
	{
		mCollider.response = ::mw::Collision::ObjectResponse(Collision::ObjectResponsePreset::CollideAll);
	}
}

Collider* Bullet::getCollider()
{
	return &mCollider;
}
}
