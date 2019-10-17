#pragma once
#include "MenuTemplate.h"

#include <SFML/Graphics/Sprite.hpp>

namespace Menu
{

	enum class Option
	{
		Play,
		Settings,
		Credits,
		Exit,
		COUNT,
	};

	static const char* OptionNames[static_cast<int>(Option::COUNT)] =
	{
		"Play",
		"Settings",
		"Credits",
		"Exit",
	};
}

class MenuState : public MenuTemplate<Menu::Option>
{
public:
	MenuState(StateStack &stateStack, const Context &context);

	virtual void draw() override;

private:
	void handleConfirmInput() override;

	void setupBackgroundSprite();

private:
	sf::Sprite mBackgroundSprite;
};

