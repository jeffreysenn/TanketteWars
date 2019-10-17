#include "AvatarActor.h"

AvatarActor::AvatarActor(
	const sf::Texture &texture, 
	Rendering::Layer layer)
	: SpriteActor(texture, layer)
{
	mCommandCategory = CommandCategory::Avatar;
}

void AvatarActor::jump()
{
	mLocalVel.y = -50;
}

void AvatarActor::goStraight()
{
	mLocalVel = sf::Vector2f(0, 0);
}

void AvatarActor::updateSelf(float deltaSeconds)
{
	setVelocity(mBaseVel + mLocalVel);

	SpriteActor::updateSelf(deltaSeconds);
}
