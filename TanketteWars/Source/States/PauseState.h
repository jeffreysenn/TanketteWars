#pragma once
#include "MenuTemplate.h"

#include <SFML/Graphics/RectangleShape.hpp>

enum class PauseOption
{
	Resume,
	GoToMenu,
	COUNT,
};

static const char* PauseOptionNames[]
{
	"Resume",
	"Main Menu",
};


class PauseState : public MenuTemplate<PauseOption>
{
public:
	PauseState(StateStack &stateStack, const Context &context);

	void draw() override;

private:
	void handleConfirmInput() override;

private:
	sf::RectangleShape mBackgroundShape;
};

