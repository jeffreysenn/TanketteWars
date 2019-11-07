#include "TanketteWarClient.h"
#include "StateBase.h"
#include "TitleState.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "CreditState.h"
#include "SettingState.h"
#include "ConnectState.h"
#include "ChoosingServerState.h"
#include "EndState.h"
#include "Context.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>  
#include <alpha.h>
#include <tankett_shared.h>
#include <cstdlib>

namespace mw
{
Game* Game::create()
{
	return new client::TanketteWarClient();
}
}
namespace client
{

TanketteWarClient::TanketteWarClient()
try
	: mWindow(
	::sf::VideoMode(1280, 720),
	"Tankket Wars",
	::sf::Style::Titlebar | ::sf::Style::Close)
	, mStateStack()
	, mFPSMeter(mStatsText)
{
	mWindow.setKeyRepeatEnabled(true);

	mTextureManager.load(Texture::Back,
						 "../Assets/sunny-land-files/environment/back.png");

	mFontManager.load(Font::Sansation, "../Assets/Fonts/Sansation.ttf");
	mStatsText.setFont(*mFontManager.get(Font::Sansation));
	mStatsText.setCharacterSize(20);
	mStatsText.setPosition(10, 10);
	mStatsText.setFillColor(::sf::Color::Magenta);

	registerStates();
	mStateStack.pushState(StateID::Title);

	Context& context = Context::getInstance();
	context.stack = &mStateStack;
	context.window = &mWindow;
	context.fontManager = &mFontManager;
	context.textureManager = &mTextureManager;
	context.mapManager = &mMapManager;
	context.networkManager = &mNetworkManager;

}
catch (const std::runtime_error & e)
{
	std::cout << "Exception: " << e.what() << std::endl;
	std::terminate();
}

const ::sf::Time updateInterval = ::sf::seconds(1.f / 60.f);
const ::sf::Time networkInterval = ::sf::seconds(1.f / (float)::tankett::PROTOCOL_SEND_PER_SEC);
void TanketteWarClient::run()
{
	::sf::Clock clock;
	::sf::Time timeSinceLastUpdate = ::sf::Time::Zero;
	::sf::Time timeSinceLastNetwork = ::sf::Time::Zero;

	while (mWindow.isOpen())
	{
		::sf::Time lastLoopDuration = clock.getElapsedTime();
		// network fixed send
		timeSinceLastNetwork += lastLoopDuration;

		while (timeSinceLastNetwork > networkInterval)
		{
			mNetworkManager.send();
			timeSinceLastNetwork -= networkInterval;
		}
		mNetworkManager.receive();

		// Fix the update interval. 
		// In case render takes longer than TimePerFrame
		lastLoopDuration = clock.restart();
		timeSinceLastUpdate += lastLoopDuration;
		while (timeSinceLastUpdate > updateInterval)
		{
			timeSinceLastUpdate -= updateInterval;
			handleInputs();
			update(1.f/60.f);

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		mFPSMeter.update(lastLoopDuration);

		render();
	}
}

void TanketteWarClient::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<GameState>(StateID::Game);
	mStateStack.registerState<PauseState>(StateID::Pause);
	mStateStack.registerState<CreditState>(StateID::Credit);
	mStateStack.registerState<SettingState>(StateID::Setting);
	mStateStack.registerState<ConnectState>(StateID::Connect);
	mStateStack.registerState<EndState>(StateID::End);
	mStateStack.registerState<ChoosingServerState>(StateID::ChoosingServer);
}

void TanketteWarClient::handleInputs()
{
	::sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		// Closing window events
		if (event.type == ::sf::Event::Closed)
			mStateStack.clearStates();

		bool& isWindowFocused = Context::getInstance().isWindowFocused;
		if (event.type == ::sf::Event::GainedFocus)
			isWindowFocused = true;
		if (event.type == ::sf::Event::LostFocus)
			isWindowFocused = false;
	}
}

void TanketteWarClient::update(float deltaSeconds)
{
	mStateStack.update(deltaSeconds);
}

void TanketteWarClient::render()
{
	mWindow.clear();

	mStateStack.draw();

	// Move to default view so that stats can be drawn at the right position
	mWindow.setView(mWindow.getDefaultView());
	// Draw stats
	mWindow.draw(mStatsText);

	mWindow.display();
}

}