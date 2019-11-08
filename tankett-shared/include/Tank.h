#pragma once
#include "Actors/Pawn.h"
#include "Bullet.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Audio/Sound.hpp>

namespace sf
{
class SoundBuffer;
}

namespace mw
{
class CameraActor;
}

namespace tankett
{
class PlayerController;

class Tank : public ::mw::Pawn
{
public:
	Tank();
	Tank(const ::sf::Texture& hullTexture, const ::sf::Texture& turretTexture, const ::sf::Texture& bulletTexture, ::sf::SoundBuffer* fireSound);
	~Tank();

	float getSpeed() const { return mSpeed; }

	void addDirection(const ::sf::Vector2f& dd) { mDirection += dd; }
	void addDirection(float ddx, float ddy) { mDirection += ::sf::Vector2f(ddx, ddy); }
	void setDirection(const ::sf::Vector2f& direction) { mDirection = direction; }
	void setDirection(float dx, float dy){ mDirection = ::sf::Vector2f(dx, dy); }
	void aimAt(const ::sf::Vector2f pos);
	void aimAt(float angle);
	void fire(uint32_t inputNum = 0);
	int getCooldown() const;

	Bullet* spawnBullet(float angle);

	void setController(PlayerController* controller) { mController = controller; }
	PlayerController* getController() const { return mController; }
	void setCamera(::mw::CameraActor* camera) { mCamera = camera; }
	::mw::CameraActor* resetCamera();

	float mousePosToAngle(const sf::Vector2f& pos) const;
	float getTurretAngle() const { return mTurretAngle; }

protected:
	virtual void reportRenderInfoSelf(::mw::Renderer& renderer, ::sf::RenderStates states) const override;
	virtual void updateSelf(float deltaSeconds) override;
	virtual void onCollisionEnter(Actor& other) override;

private:
	::sf::Sprite mBarrelSprite;
	::mw::Rendering::Layer mTurretLayer;
	const float mSpeed;
	::sf::Vector2f mDirection;
	::sf::Clock mFireClock;
	::sf::Time mLastFireTime;
	const ::sf::Texture* mBulletTexture;
	float mTurretAngle;
	// TODO: decrease dependency
	PlayerController* mController;
	::mw::CameraActor* mCamera;
	uint32_t mLastFireInputNum{};
	::sf::Sound mFireSound;

};
}