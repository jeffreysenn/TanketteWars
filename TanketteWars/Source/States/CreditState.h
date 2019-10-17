#pragma once
#include "MenuTemplate.h"

#include <SFML/Graphics/RectangleShape.hpp>

namespace Credit
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


class CreditState : public MenuTemplate<Credit::Option>
{
public:
	CreditState(StateStack &stateStack, const Context &context);

	void draw() override;

private:
	void handleConfirmInput() override;

	sf::RectangleShape mBackgroundShape;
	sf::Text mCreditText;
};

