#include "MenuState.h"
#include "Context.h"
#include "ClientStateStack.h"
namespace client
{
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
	ClientStateStack& stack = *Context::getInstance().stack;

	switch (getCurrentOption())
	{
	case Menu::Option::Connect:
		stack.pushState(StateID::ChoosingServer);
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
	auto& backgroungTexture = *Context::getInstance().textureManager->get(Texture::Back);
	backgroungTexture.setRepeated(true);
	mBackgroundSprite.setTexture(backgroungTexture);
	::sf::IntRect rect(0, 0, getRenderWindow().getSize().x, getRenderWindow().getSize().y);
	// TODO: make scale adaptable to different resolutions
	mBackgroundSprite.setScale(4, 4);
}
}