#include "TitleState.h"

#include "../Helpers/GraphicsHelper.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#define PI 3.14159265

TitleState::TitleState(StateStack & stateStack, const Context & context)
	: State(stateStack, context)
	, mTextTime(0)
{
	setupContinueText();
	setupTitleText();
}

TitleState::~TitleState()
{
}

bool TitleState::update(float deltaSeconds)
{
	continueTextBlink(deltaSeconds);
	return false;
}

bool TitleState::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return false;
}

void TitleState::draw()
{
	getRenderWindow().draw(mTitleText);
	getRenderWindow().draw(mContinueText);
}

void TitleState::setupContinueText()
{
	mContinueText.setString("Press any key to Continue");
	mContinueText.setFont(*getContext().fontManager->get(Font::MineCraft));
	mContinueText.setFillColor(sf::Color::White);
	GraphicsHelper::centreOrigin(mContinueText);
	sf::Vector2u windowSize(getRenderWindow().getSize());
	mContinueText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y - 100);
}

void TitleState::setupTitleText()
{
	mTitleText.setString("Fancy Title Screen");
	mTitleText.setCharacterSize(50);
	mTitleText.setFont(*getContext().fontManager->get(Font::MineCraft));
	mTitleText.setFillColor(sf::Color::Cyan);
	GraphicsHelper::centreOrigin(mTitleText);
	sf::Vector2u windowSize(getRenderWindow().getSize());
	mTitleText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);

}

void TitleState::continueTextBlink(float deltaSeconds)
{
	mTextTime += deltaSeconds;
	if (mTextTime > PI)
		mTextTime = 0;
	float textAlpha = abs(sin(mTextTime));

	sf::Color textColor(mContinueText.getFillColor());
	sf::Color newColor(textColor.a, textColor.g, textColor.b,
		(sf::Uint8) (textAlpha * 255));
	mContinueText.setFillColor(newColor);
}
