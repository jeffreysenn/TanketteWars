#pragma once
#include "Actor.h"

#include <SFML/System.hpp>


class MovableActor : public Actor
{
public:
	MovableActor();

	void setVelocity(const sf::Vector2f &velocity) { mVelocity = velocity; }
	void setVelocity(float vx, float vy) { mVelocity = sf::Vector2f(vx, vy); }
	void addVelocity(const sf::Vector2f& dv) { mVelocity += dv; }
	void addVelocity(float dvx, float dvy) { mVelocity += sf::Vector2f(dvx, dvy); }

	sf::Vector2f getVelocity() const override { return mVelocity; }
	sf::Vector2f getWorldVelocity() const override;

protected:
	virtual void updateSelf(float deltaSeconds) override;

protected:
	sf::Vector2f mVelocity;
};

