#include "MenuState.h"
#include "ClientContext.h"
#include "ClientStateStack.h"

MenuState::MenuState()
	: MenuTemplate(Menu::OptionNames, static_cast<int>(Menu::Option::COUNT))
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
	ClientStateStack& stack = *ClientContext::getInstance().stack;

	switch (getCurrentOption())
	{
	case Menu::Option::Play:
		stack.popState();
		stack.pushState(StateID::Game);
		break;
	case Menu::Option::Connect:
		stack.pushState(StateID::Connect);
		break;
	case Menu::Option::Settings:
		stack.pushState(StateID::Setting);
		break;
	case Menu::Option::Credits:
		stack.pushState(StateID::Credit);
		break;
	case Menu::Option::Exit:
		stack.clearStates();
		break;
	default:
		break;
	}
}

void MenuState::setupBackgroundSprite()
{
	auto & backgroungTexture = *ClientContext::getInstance().textureManager->get(Texture::Back);
	backgroungTexture.setRepeated(true);
	mBackgroundSprite.setTexture(backgroungTexture);
	sf::IntRect rect(0, 0, getRenderWindow().getSize().x, getRenderWindow().getSize().y);
	// TODO: make scale adaptable to different resolutions
	mBackgroundSprite.setScale(3, 3);
}