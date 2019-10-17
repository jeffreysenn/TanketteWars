#include "MenuState.h"

MenuState::MenuState(StateStack & stateStack, const Context & context)
	: MenuTemplate(stateStack, context, Menu::OptionNames, static_cast<int>(Menu::Option::COUNT))
{
	setupBackgroundSprite();
}

void MenuState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	getRenderWindow().draw(mBackgroundSprite);
	drawOptions();
}

void MenuState::handleConfirmInput()
{
	switch (getCurrentOption())
	{
	case Menu::Option::Play:
		requestStackPop();
		requestStackPush(StateID::Game);
		break;
	case Menu::Option::Settings:
		requestStackPush(StateID::Setting);
		break;
	case Menu::Option::Credits:
		requestStackPush(StateID::Credit);
		break;
	case Menu::Option::Exit:
		requestStackPop();
		break;
	default:
		break;
	}
}

void MenuState::setupBackgroundSprite()
{
	auto & backgroungTexture = *getContext().textureManager->get(Texture::Back);
	backgroungTexture.setRepeated(true);
	mBackgroundSprite.setTexture(backgroungTexture);
	sf::IntRect rect(0, 0, getRenderWindow().getSize().x, getRenderWindow().getSize().y);
	// TODO: make scale adaptable to different resolutions
	mBackgroundSprite.setScale(3, 3);
}