#include "GameState.h"
#include "Context.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
#include "Unit.h"

namespace server
{

GameState::GameState()
	: mNetworkManager(*Context::getInstance().networkManager)
{

}

void GameState::processMessages()
{
	checkJoin();

	auto& clients = mNetworkManager.getClients();
	for (auto& client : clients)
	{
		if (client.second.sendMessageQueue.size() == 0)
		{
			message_server_to_client* msgS2C = new message_server_to_client;
			msgS2C->game_state = GAME_STATE::ROUND_RUNNING;
			msgS2C->client_count = (uint8)clients.size();
			server_to_client_data data;
			data.client_id = client.second.id;
			mNetworkManager.pushMessage(client.first, msgS2C);
		}

	}
}

void GameState::checkJoin()
{
	auto& clients = mNetworkManager.getClients();

	for (auto& client : clients)
	{
		// insert controllers
		if (mControllers.find(client.first) == mControllers.end())
		{
			::mw::CommandCategory commandCategory =
				(::mw::CommandCategory)((uint32_t)::mw::CommandCategory::Tank0 << client.second.id);
			::tankett::PlayerController controller(commandCategory);
			controller.spawnTankServer(mWorld.getCommandQueue(), 0);
			mControllers.insert(::std::make_pair(client.first, controller));
		}
	}
}

}