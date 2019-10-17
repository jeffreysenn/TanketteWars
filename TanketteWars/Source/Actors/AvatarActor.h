#pragma once
#include "SpriteActor.h"

class AvatarActor :
	public SpriteActor
{
public:
	explicit AvatarActor(const sf::Texture& texture, 
				Rendering::Layer layer = Rendering::Default);

	void setBaseVelocity(const sf::Vector2f &baseVel) { mBaseVel = baseVel; }
	void setBaseVelocity(float vx, float vy) { mBaseVel = sf::Vector2f(vx, vy); }
	const sf::Vector2f getBaseVelocity() const { return mBaseVel; }

	void setLocalVelocity(const sf::Vector2f &localVel) { mLocalVel = localVel; }
	void setLocalVelocity(float vx, float vy) { mLocalVel = sf::Vector2f(vx, vy); }
	const sf::Vector2f getLocalVelocity() const { return mLocalVel; }

	void stopMoving() { mLocalVel = -mBaseVel; }

	void jump();
	void goStraight();

protected:
	virtual void updateSelf(float deltaSeconds) override;

private:
	sf::Vector2f mBaseVel = sf::Vector2f();
	sf::Vector2f mLocalVel = sf::Vector2f();
};

