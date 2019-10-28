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

	server_to_client_data dataArr[4];
	for (auto& client : clients)
	{
		uint8_t id = client.second.id;
		dataArr[id].client_id = id;
		dataArr[id].angle = mControllers[id].getTankTurretAngle();
		auto pos = mControllers[id].getTankPosition();
		dataArr[id].position = alpha::vector2(pos.x, pos.y);
	}

	for (auto& client : clients)
	{
		if(client.second.sendMessageQueue.size() != 0)
			continue;

		message_server_to_client* message = new message_server_to_client;
		message->receiver_id = client.second.id;
		message->game_state = GAME_STATE::ROUND_RUNNING;
		message->client_count = (uint8_t)clients.size();
		memcpy(message->client_data, dataArr, sizeof(dataArr));

		mNetworkManager.pushMessage(client.first, message);
	}

}

void GameState::checkJoin()
{
	auto& clients = mNetworkManager.getClients();


	for (auto& client : clients)
	{
		uint8_t id = client.second.id;
		// insert controllers
		if (mControllers.find(id) == mControllers.end())
		{
			::mw::CommandCategory commandCategory =
				(::mw::CommandCategory)((uint32_t)::mw::CommandCategory::Tank0 << client.second.id);
			::tankett::PlayerController controller(client.second.id);
			mControllers.insert(::std::make_pair(id, controller));
			mControllers[id].spawnTank_server(mWorld.getTankManager());
		}
	}
}

}