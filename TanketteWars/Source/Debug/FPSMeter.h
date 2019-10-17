#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class FPSMeter
{
public:
	FPSMeter(sf::Text &text);

	void update(sf::Time lastLoopDuration);
	void updateText(sf::Text text);

private:
	sf::Text* mText = nullptr;
	sf::Time mTimeSinceLastUpdate = sf::Time::Zero;
	uint32_t mFrameCount = 0;
	const sf::Time mUpdateInterval = sf::seconds(.5f);
};

