#pragma once
#include "MenuTemplate.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
namespace client
{

enum class PauseOption
{
	Resume,
	Disconnect,
	COUNT,
};

static const char* PauseOptionNames[]
{
	"Resume",
	"Disconnect",
};


class PauseState : public MenuTemplate<PauseOption>
{
public:
	PauseState();

	void draw() override;
	bool update(float deltaSeconds) override;

private:
	void handleConfirmInput() override;

private:
	::sf::RectangleShape mBackgroundShape;
	bool mDisconnecting;
	::sf::Clock mDisconnectClock;
	::sf::Text mDisconnectingText;
};


}