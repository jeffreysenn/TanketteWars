#include "Tank.h"
#include "Rendering/Renderer.h"
#include "Unit.h"
#include "Helpers/Helper.h"
#include "PlayerController.h"
#include "Actors/CameraActor.h"

namespace tankett
{
Tank::Tank()
	: mSpeed(unit::unit2pix(4))
	, mBulletTexture(nullptr)
	, mLastFireTime(::sf::seconds(-100))
	, mTurretAngle(0)
	, mTurretLayer(Rendering::Turret)
	, mController(nullptr)
	, mCamera(nullptr)
{
	float width = unit::unit2pix(1);
	float height = unit::unit2pix(1);
	const ::sf::FloatRect colliderRect(-width / 2,
									   -height / 2,
									   width,
									   height);
	auto collider = std::make_unique<Collider>(Collision::ObjectType::Dynamic, 
											   Collision::ObjectResponsePreset::CollideAll,
											   colliderRect);
	setCollider(collider);
}

Tank::Tank(const ::sf::Texture& hullTexture, const ::sf::Texture& turretTexture, const ::sf::Texture& bulletTexture)
	: Pawn(1, hullTexture, Rendering::Default)
	, mBarrelSprite(turretTexture)
	, mTurretLayer(Rendering::Turret)
	, mSpeed(unit::unit2pix(4))
	, mBulletTexture(&bulletTexture)
	, mTurretAngle(0)
	, mLastFireTime(::sf::seconds(-100))
	, mController(nullptr)
	, mCamera(nullptr)
	//, mDebugCircle(5)
{
	::sf::FloatRect spriteBounds(mBarrelSprite.getLocalBounds());
	mBarrelSprite.setOrigin(spriteBounds.width / 2, 0);

	float width = unit::unit2pix(1);
	float height = unit::unit2pix(1);
	const ::sf::FloatRect colliderRect(-width / 2,
									   -height / 2,
									   width,
									   height);
	auto collider = std::make_unique<Collider>(Collision::ObjectType::Dynamic,
											   Collision::ObjectResponsePreset::CollideAll,
											   colliderRect);
	setCollider(collider);
}

Tank::~Tank()
{
	for (auto& bullet : mBullets)
	{
		bullet->resetOwner();
	}

	if (mController)
		mController->unpossess();
	if (mCamera)
		mCamera->detach();
}

void Tank::aimAt(const ::sf::Vector2f pos)
{
	::sf::Vector2f dir = pos - getWorldPosition();
	float length = helper::Vector::getLength(dir);
	if (length < 0.01f)
		return;

	auto angleRads = ::std::atan2(dir.y, dir.x);
	mTurretAngle = helper::Vector::rad2deg(angleRads);

	mBarrelSprite.setRotation(mTurretAngle - 90.f);
}

void Tank::aimAt(float angle)
{
	mTurretAngle = angle;
	mBarrelSprite.setRotation(angle - 90.f);
}

constexpr uint32_t cooldown = 1000;
uint8_t bulletID = 0;
void Tank::fire()
{
	auto timeNow = mFireClock.getElapsedTime();
	if (timeNow - mLastFireTime > ::sf::milliseconds(cooldown))
	{
		if (mBullets.empty()) bulletID = 0;
		mLastFireTime = timeNow;
		spawnBullet()->setID(bulletID);
		++bulletID;
	}
}

constexpr int bulletSpeed = 8;
Bullet* Tank::spawnBullet()
{

	::std::unique_ptr<Bullet> bullet;
	if (mBulletTexture)
		bullet = ::std::make_unique<Bullet>(*mBulletTexture, this);
	else
		bullet = ::std::make_unique<Bullet>(this);

	Bullet* ptr = bullet.get();
	bullet->setPosition(getWorldPosition());
	bullet->setRotation(mTurretAngle);
	auto dir = helper::Vector::deg2vec(mTurretAngle);
	::sf::Vector2f relativeVel(dir.x * unit::unit2pix(bulletSpeed),
							   dir.y * unit::unit2pix(bulletSpeed));
	bullet->setVelocity(getWorldVelocity() + relativeVel);
	getSceneGraph()->attachChild(::std::move(bullet));

	return ptr;
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

::mw::CameraActor* Tank::resetCamera()
{
	auto temp = mCamera;
	mCamera = nullptr;
	return temp;
}

float Tank::mousePosToAngle(const sf::Vector2f& pos) const
{
	::sf::Vector2f dir = pos - getWorldPosition();
	float length = helper::Vector::getLength(dir);
	if (length < 0.01f)
		return 0;

	auto angleRads = ::std::atan2(dir.y, dir.x);
	float angle = helper::Vector::rad2deg(angleRads);

	return angle;
}

void Tank::reportRenderInfoSelf(Renderer& renderer, ::sf::RenderStates states) const
{
	Pawn::reportRenderInfoSelf(renderer, states);

	RenderInfo renderInfo(mBarrelSprite, states);
	renderer.pushRenderInfo(renderInfo, Rendering::Turret);

	//RenderInfo debugRenderInfo(mDebugCircle, states);
	//renderer.pushRenderInfo(debugRenderInfo, Rendering::UI);
}

void Tank::updateSelf(float deltaSeconds)
{
	auto direction = helper::Vector::normalize(mDirection);
	if (direction != ::sf::Vector2f())
	{
		float angleRads = (float)::std::atan2(direction.y, direction.x);
		auto angleDegs = helper::Vector::rad2deg(angleRads) - 90;
		getSprite()->setRotation((float)angleDegs);
	}
	setVelocity(direction * mSpeed);
	Pawn::updateSelf(deltaSeconds);

	mDirection = ::sf::Vector2f();
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

}