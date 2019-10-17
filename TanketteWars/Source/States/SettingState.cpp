#include "SettingState.h"

SettingState::SettingState(StateStack & stateStack, const Context & context)
	: MenuTemplate<Setting::Option>(stateStack, context, Setting::OptionNames, static_cast<int>(Setting::Option::COUNT))
{
	mBackgroundShape.setFillColor(sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(sf::Vector2f(getRenderWindow().getSize()));

	mCreditText.setFont(*context.fontManager->get(Font::MineCraft));
	mCreditText.setString("WIP");
	mCreditText.setFillColor(sf::Color::Blue);
	mCreditText.setCharacterSize(50);
	GraphicsHelper::centreOrigin(mCreditText);
	sf::Vector2u windowSize(getRenderWindow().getSize());
	mCreditText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

void SettingState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());

	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mCreditText);

	MenuTemplate<Setting::Option>::draw();
}

void SettingState::handleConfirmInput()
{
	requestStackPop();
}