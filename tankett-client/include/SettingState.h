#pragma once
#include "MenuTemplate.h"

#include <SFML/Graphics/RectangleShape.hpp>

using namespace mw;

namespace Setting
{
	enum class Option
	{
		Back,
		COUNT,
	};

	static const char* OptionNames[static_cast<int>(Option::COUNT)] =
	{
		"Back",
	};
}

class SettingState : public MenuTemplate<Setting::Option>
{
public:
	SettingState();

	void draw() override;

private:
	void handleConfirmInput() override;

	sf::RectangleShape mBackgroundShape;
	sf::Text mCreditText;
};

