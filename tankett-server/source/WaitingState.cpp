#include "WaitingState.h"
#include "Context.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
namespace server
{

WaitingState::WaitingState()
{
}

void WaitingState::processMessages()
{
	NetworkManager& networkManager = *Context::getInstance().networkManager;
	auto& clients = networkManager.getClients();

	if (clients.size() < 2)
	{
		for (auto& client : clients)
		{
			if (client.second.sendMessageQueue.size() == 0)
			{
				message_server_to_client* msgS2C = new message_server_to_client;
				msgS2C->game_state = GAME_STATE::WAITING_FOR_PLAYER;
				msgS2C->client_count = (uint8)clients.size();
				networkManager.pushMessage(client.first, msgS2C);
			}
		}
	}
	else
	{
		auto& stack = *Context::getInstance().stack;
		stack.clearStates();
		stack.pushState(GAME_STATE::ROUND_RUNNING);
	}

	networkManager.clearAllClientsReceivedMessages();
}

}