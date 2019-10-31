#include "ConnectState.h"
#include "NetworkManager.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "tankett_shared.h"
namespace client
{

ConnectState::ConnectState()
	: MenuTemplate<Connect::Option>(Connect::OptionNames, static_cast<int>(Connect::Option::COUNT))
	, mNetworkManager(*Context::getInstance().networkManager)
{
	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));

	mConnectText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mConnectText.setString("Connecting");
	mConnectText.setFillColor(::sf::Color::Blue);
	mConnectText.setCharacterSize(50);
	helper::Graphics::centreOrigin(mConnectText);
	::sf::Vector2u windowSize(getRenderWindow().getSize());
	mConnectText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

bool ConnectState::update(float deltaSeconds)
{
	NetworkManager::ConnectionState netState = mNetworkManager.getState();
	if (netState == NetworkManager::ConnectionState::None)
		mNetworkManager.setState(NetworkManager::ConnectionState::Discovering);

	if (netState == NetworkManager::ConnectionState::Connected)
	{
		mConnectText.setString("Connected to server\nWaiting for players");
		helper::Graphics::centreOrigin(mConnectText);

		auto& receivedMessages = mNetworkManager.getReceivedMessages();
		for (auto& receivedMessage : receivedMessages)
		{
			::tankett::network_message_type type = (::tankett::network_message_type)receivedMessage->type_;
			switch (type)
			{
			case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
			{
				::tankett::message_server_to_client* mS2C = static_cast<::tankett::message_server_to_client*>(receivedMessage.get());

				if (mS2C->game_state == ::tankett::GAME_STATE::ROUND_RUNNING)
				{
					ClientStateStack& stack = *Context::getInstance().stack;
					stack.clearStates();
					stack.pushState(StateID::Game);
				}

			} break;
			}
		}

		receivedMessages.clear();
	}

	return false;
}

void ConnectState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());

	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mConnectText);
	MenuTemplate<Connect::Option>::draw();
}

void ConnectState::handleConfirmInput()
{
	ClientStateStack& stack = *Context::getInstance().stack;
	stack.popState();
}

}