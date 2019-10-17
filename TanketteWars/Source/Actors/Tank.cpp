#include "Tank.h"
#include "../Rendering/Renderer.h"
#include "../Units/Units.h"
#include "../Helpers/VectorHelper.h"

Tank::Tank(const sf::Texture& hullTexture, const sf::Texture& turretTexture, const sf::Texture& bulletTexture)
	: Pawn(1, hullTexture, Rendering::Default)
	, mBarrelSprite(turretTexture)
	, mTurretLayer(Rendering::Turret)
	, mSpeed(Units::unit2pix(4))
	, mBulletTexture(bulletTexture)
	, mTurretAngle(0)
	, mCollider(Collision::ObjectType::Dynamic, Collision::ObjectResponsePreset::CollideAll)
	, mLastFireTime(sf::seconds(-100))
	//, mDebugCircle(5)
{
	sf::FloatRect spriteBounds(mBarrelSprite.getLocalBounds());
	mBarrelSprite.setOrigin(spriteBounds.width / 2, 0);

	sf::FloatRect hullSpriteBounds(getSprite()->getLocalBounds());
	const sf::FloatRect colliderRect(-hullSpriteBounds.width/2, 
									 -hullSpriteBounds.height/2, 
									 Units::unit2pix(1), 
									 Units::unit2pix(1));
	mCollider.rect = colliderRect;
}

Tank::~Tank()
{
	for (auto& bullet : mBullets)
	{
		bullet->resetOwner();
	}
}

void Tank::aimAt(const sf::Vector2f pos)
{
	sf::Vector2f dir = pos - getWorldPosition();
	float length = VectorHelper::getLength(dir);
	if (length < 0.01f)
		return;

	auto angleRads = std::atan2(dir.y, dir.x);
	mTurretAngle = VectorHelper::rad2deg(angleRads);

	mBarrelSprite.setRotation(mTurretAngle - 90.f);
}

constexpr uint32_t cooldown = 1000;
void Tank::fire()
{
	auto timeNow = mFireClock.getElapsedTime();
	if (timeNow - mLastFireTime > sf::milliseconds(cooldown))
	{
		mLastFireTime = timeNow;
		spawnBullet();
	}
}

constexpr int bulletSpeed = 8;
void Tank::spawnBullet()
{
	auto bullet = std::make_unique<Bullet>(mBulletTexture, this);
	addBullet(bullet.get());
	bullet->setPosition(getWorldPosition());
	bullet->setRotation(mTurretAngle);
	auto dir = VectorHelper::deg2vec(mTurretAngle);
	sf::Vector2f relativeVel(dir.x * Units::unit2pix(bulletSpeed),
							 dir.y * Units::unit2pix(bulletSpeed));
	bullet->setVelocity(getWorldVelocity() + relativeVel);
	getSceneGraph()->attachChild(std::move(bullet));
}

bool Tank::addBullet(Bullet* bullet)
{
	if (getBullet(bullet) != -1)
		return false;

	mBullets.push_back(bullet);
	return true;
}

bool Tank::removeBullet(Bullet* bullet)
{
	int index = getBullet(bullet);
	if (index != -1)
	{
		mBullets.erase(mBullets.begin() + index);
		return true;
	}
	return false;
}

int Tank::getBullet(Bullet* bullet)
{
	for (auto it = mBullets.begin(); it != mBullets.end(); ++it)
	{
		if (*it == bullet)
		{
			return static_cast<int>(it - mBullets.begin());
		}
	}

	return -1;
}

void Tank::reportRenderInfoSelf(Renderer& renderer, sf::RenderStates states) const
{
	Pawn::reportRenderInfoSelf(renderer, states);

	RenderInfo renderInfo(mBarrelSprite, states);
	renderer.pushRenderInfo(renderInfo, Rendering::Turret);

	//RenderInfo debugRenderInfo(mDebugCircle, states);
	//renderer.pushRenderInfo(debugRenderInfo, Rendering::UI);
}

void Tank::updateSelf(float deltaSeconds)
{
	auto direction = VectorHelper::normalize(mDirection);
	if (direction != sf::Vector2f())
	{
		auto angleRads = std::atan2(direction.y, direction.x);
		auto angleDegs = angleRads * 180.0 / PI - 90;
		getSprite()->setRotation((float) angleDegs);
	}
	setVelocity(direction * mSpeed);
	Pawn::updateSelf(deltaSeconds);

	mDirection = sf::Vector2f();
}

void Tank::onCollisionEnter(Actor& other)
{
	Bullet* bullet = dynamic_cast<Bullet*>(&other);
	if (bullet)
	{
		int index = getBullet(bullet);
		if (index == -1)
		{
			destroy(this);
		}
	}
	
	Tank* tank = dynamic_cast<Tank*>(&other);
	if (tank)
	{
		destroy(this);
	}
}
