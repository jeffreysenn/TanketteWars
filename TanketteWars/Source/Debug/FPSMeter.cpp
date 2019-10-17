#include "FPSMeter.h"

FPSMeter::FPSMeter(sf::Text &text)
	: mText(&text)
{
}

void FPSMeter::update(sf::Time lastLoopDuration)
{
	mFrameCount++;
	mTimeSinceLastUpdate += lastLoopDuration;
	while (mTimeSinceLastUpdate > mUpdateInterval)
	{
		uint32_t fps = (uint32_t)(mFrameCount / mUpdateInterval.asSeconds());
		uint32_t ft;
		if (mFrameCount == 0)
			ft = (uint32_t) mUpdateInterval.asMicroseconds();
		else 
			ft = (uint32_t)(mTimeSinceLastUpdate.asMicroseconds() / mFrameCount);

		if (mText)
		{
			mText->setString("FPS : " + std::to_string(fps) + "\n" +
					   "FT : " + std::to_string(ft) + " us");
		}
		mTimeSinceLastUpdate -= mUpdateInterval;
		mFrameCount = 0;
	}
}