#pragma once
#include "Pawn.h"
#include "Bullet.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/CircleShape.hpp>
namespace mw
{

	class Tank : public Pawn
	{
	public:
		Tank(const ::sf::Texture& hullTexture, const ::sf::Texture& turretTexture, const ::sf::Texture& bulletTexture);
		~Tank();

		float getSpeed() const { return mSpeed; }

		void addDirection(const ::sf::Vector2f& dd) { mDirection += dd; }
		void addDirection(float ddx, float ddy) { mDirection += ::sf::Vector2f(ddx, ddy); }
		void aimAt(const ::sf::Vector2f pos);
		void fire();

		void spawnBullet();
		bool addBullet(Bullet* bullet);
		bool removeBullet(Bullet* bullet);
		int getBullet(Bullet* bullet);

	protected:
		virtual void reportRenderInfoSelf(class Renderer& renderer, ::sf::RenderStates states) const override;
		virtual void updateSelf(float deltaSeconds) override;
		virtual Collider* getCollider() { return &mCollider; }
		virtual void onCollisionEnter(Actor& other) override;

	private:
		::sf::Sprite mBarrelSprite;
		Rendering::Layer mTurretLayer;
		const float mSpeed;
		::sf::Vector2f mDirection;
		::sf::Clock mFireClock;
		::sf::Time mLastFireTime;
		const ::sf::Texture& mBulletTexture;
		float mTurretAngle;
		/*
					/
				   /
				  / turrentAngle
				 /______________
		*/

		Collider mCollider;
		::std::vector<Bullet*> mBullets;

		//::sf::CircleShape mDebugCircle;
	};

}