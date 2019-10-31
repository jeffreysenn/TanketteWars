#pragma once
#include "MenuTemplate.h"

#include <SFML/Graphics/Sprite.hpp>
namespace client
{

namespace Menu
{
enum class Option
{
	Connect,
	Settings,
	Credits,
	Exit,
	COUNT,
};

static const char* OptionNames[static_cast<int>(Option::COUNT)] =
{
	"Connect",
	"Settings",
	"Credits",
	"Exit",
};
}

class MenuState : public MenuTemplate<Menu::Option>
{
public:
	MenuState();

	virtual void draw() override;

private:
	void handleConfirmInput() override;

	void setupBackgroundSprite();

private:
	::sf::Sprite mBackgroundSprite;
};


}