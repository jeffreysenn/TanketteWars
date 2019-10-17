#include "CreditState.h"
#include "../Helpers/GraphicsHelper.h"

CreditState::CreditState(StateStack & stateStack, const Context & context)
	: MenuTemplate<Credit::Option>(stateStack, context, Credit::OptionNames, static_cast<int>(Credit::Option::COUNT))
{
	mBackgroundShape.setFillColor(sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(sf::Vector2f(getRenderWindow().getSize()));

	mCreditText.setFont(*context.fontManager->get(Font::MineCraft));
	mCreditText.setString("Designer and Programmer \n\nXiaoyue Chen");
	mCreditText.setFillColor(sf::Color::Blue);
	mCreditText.setCharacterSize(50);
	GraphicsHelper::centreOrigin(mCreditText);
	sf::Vector2u windowSize(getRenderWindow().getSize());
	mCreditText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

void CreditState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());

	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mCreditText);

	MenuTemplate<Credit::Option>::draw();
}

void CreditState::handleConfirmInput()
{
	requestStackPop();
}
