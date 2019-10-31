#include "Bullet.h"
#include "Rendering/Renderer.h"
#include "Unit.h"
#include "PlayerController.h"
#include "Tank.h"

namespace tankett
{
Bullet::Bullet(PlayerController* owner)
	: mOwner(owner)
	, mID(0)
{
	owner->addBullet(this);
	registerCollider();
}

Bullet::Bullet(const ::sf::Texture& texture, PlayerController* owner)
	: SpriteActor(texture, ::mw::Rendering::Bullet)
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
	Tank* otherTank = dynamic_cast<Tank*>(&other);
	if (otherTank)
	{
		if (otherTank == mOwner->getPossessedTank())
		{
			return;
		}
		else
		{
			mOwner->addScore(1);
		}
	}

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
	auto collider = ::std::make_unique<::mw::Collider>(::mw::Collision::ObjectType::Dynamic, ::mw::Collision::ObjectResponsePreset::CollideAll, colliderRect);
	setCollider(collider);
}
}
