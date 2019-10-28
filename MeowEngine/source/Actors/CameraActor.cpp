#include "Actors/CameraActor.h"

namespace mw
{
CameraActor::CameraActor()
	: MovableActor()
{
}


CameraActor::~CameraActor()
{
}

void CameraActor::updateSelf(float deltaSeconds)
{
	MovableActor::updateSelf(deltaSeconds);

	if (mPossessedActor)
		setWorldPosition(mPossessedActor->getWorldPosition());

	::sf::View::setCenter(getWorldPosition());
}

}