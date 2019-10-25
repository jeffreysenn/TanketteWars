#include "Game.h"

#if 0

#include "States/ClientState.h"
#include "States/TitleState.h"
#include "States/MenuState.h"
#include "States/GameState.h"
#include "States/PauseState.h"
#include "States/CreditState.h"
#include "States/SettingState.h"
#include "States/ConnectState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>  
#include <alpha.h>

const sf::Time updateInterval = sf::seconds(1.f / 60.f);
const sf::Time networkInterval = sf::seconds(1.f / 30.f);

Game::Game()
try : mWindow(
	sf::VideoMode(960, 540),
	"Tankket Wars",
	sf::Style::Titlebar | sf::Style::Close)
	, mPlayerController(mWindow)
	, mStateStack(Context(mWindow, mTextureManager, mFontManager, mMapManager, mPlayerController))
	, mFPSMeter(mStatsText)
	, mbPaused(false)
{
	mWindow.setKeyRepeatEnabled(true);

	mTextureManager.load(Texture::Back,
			 "../Assets/sunny-land-files/environment/back.png");

	mFontManager.load(Font::MineCraft, "../Assets/Fonts/Minecraft.ttf");
	mStatsText.setFont(*mFontManager.get(Font::MineCraft));
	mStatsText.setCharacterSize(20);
	mStatsText.setPosition(10, 10);
	mStatsText.setFillColor(sf::Color::Magenta);

	registerStates();
	mStateStack.pushState(StateID::Title);
}
catch (const std::runtime_error& e)
{
	std::cout << "Exception: " << e.what() << std::endl;
	std::terminate();
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timeSinceLastNetwork = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time lastLoopDuration = clock.restart();
		timeSinceLastUpdate += lastLoopDuration;

		// Fix the update interval. 
		// In case render takes longer than TimePerFrame
		while (timeSinceLastUpdate > updateInterval)
		{
			timeSinceLastUpdate -= updateInterval;
			handleInputs();
			update(updateInterval.asSeconds());

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		timeSinceLastNetwork += lastLoopDuration;
		while (timeSinceLastNetwork > networkInterval)
		{
			timeSinceLastNetwork -= networkInterval;
		}

		mFPSMeter.update(lastLoopDuration);

		render();
	}
}

void Game::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<GameState>(StateID::Game);
	mStateStack.registerState<PauseState>(StateID::Pause);
	mStateStack.registerState<CreditState>(StateID::Credit);
	mStateStack.registerState<SettingState>(StateID::Setting);
	mStateStack.registerState<ConnectState>(StateID::Connect);

}

void Game::handleInputs()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		// Closing window events
		if (event.type == sf::Event::Closed)
			mStateStack.clearStates();

		// Focus events
		if (event.type == sf::Event::LostFocus)
			mbPaused = true;
		else if (event.type == sf::Event::GainedFocus)
			mbPaused = false;
	}
}

void Game::update(float deltaSeconds)
{
	if (mbPaused)
		return;
	mStateStack.update(deltaSeconds);
}

void Game::render()
{
	mWindow.clear();

	mStateStack.draw();

	// Move to default view so that stats can be drawn at the right position
	mWindow.setView(mWindow.getDefaultView());
	// Draw stats
	mWindow.draw(mStatsText);

	mWindow.display();
}

#endif
