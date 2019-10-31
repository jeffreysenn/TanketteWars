#include "GameState.h"
#include "Context.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
#include "Unit.h"
#include "Tank.h"
#include "Bullet.h"
#include "tankett_debug.h"
#include <vector>

namespace server
{

constexpr float ROUND_LENGTH = 90.f;

GameState::GameState()
	: mNetworkManager(*Context::getInstance().networkManager)
	, mRoundClock()
{
}

void GameState::processMessages()
{
	checkJoin();
	if (!mNetworkManager.allClientReceivedMessagesEmpty())
	{
		applyInput();
	}
	packGameState();
	checkTime();
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

void GameState::applyInput()
{
	auto& clients = mNetworkManager.getClients();

	for (auto& client : clients)
	{
		const auto& receivedMessages = client.second.receivedMessages;
		uint32_t inputCount = 0;
		for (const auto& msg : receivedMessages)
		{
			network_message_type type = (network_message_type)msg->type_;
			if (type == ::tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER)
				++inputCount;
		}

		for (const auto& msg : receivedMessages)
		{
			network_message_type type = (network_message_type)msg->type_;
			switch (type)
			{
			case ::tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER:
			{
				message_client_to_server* msgC2S = (message_client_to_server*)msg;
				if (!msgC2S) break;
				auto& controller = mControllers[client.second.id];
				bool up = msgC2S->get_input(message_client_to_server::UP);
				bool down = msgC2S->get_input(message_client_to_server::DOWN);
				bool left = msgC2S->get_input(message_client_to_server::LEFT);
				bool right = msgC2S->get_input(message_client_to_server::RIGHT);
				bool fire = msgC2S->get_input(message_client_to_server::SHOOT);
				float aimAngle = msgC2S->turret_angle;
				float deltaSeconds = 1.f / (float)PROTOCOL_SEND_PER_SEC / (float)inputCount;
				controller.updateTank(up, down, left, right, fire, aimAngle, deltaSeconds, msgC2S->input_number);
			} break;
			default:
				break;
			}
		}
	}

	mNetworkManager.clearAllClientsReceivedMessages();

}

void GameState::packGameState()
{
	auto& clients = mNetworkManager.getClients();

	server_to_client_data dataArr[4];
	for (auto& client : clients)
	{
		uint8_t id = client.second.id;
		auto& data = dataArr[id];
		data.client_id = id;
		data.ping = client.second.ping;
		data.eliminations = mControllers[id].getScore();
		const auto tank = mControllers[id].getPossessedTank();
		if (tank)
		{
			data.alive = true;
			data.angle = tank->getTurretAngle();
			const auto& pos = tank->getPosition();
			data.position = alpha::vector2(pos.x, pos.y);
		}
		else
		{
			data.alive = false;
		}

		const auto& bullets = mControllers[id].getBullets();
		data.bullet_count = (uint8_t)bullets.size();
		for (int i = 0; i < bullets.size(); ++i)
		{
			bullet_data bulletData;
			bulletData.id = bullets[i]->getID();
			const auto bulletPos = bullets[i]->getPosition();
			bulletData.position = ::alpha::vector2(bulletPos.x, bulletPos.y);
			data.bullets[i] = bulletData;
		}
	}

	for (auto& client : clients)
	{
		if (client.second.sendMessageQueue.size() != 0)
			continue;

		message_server_to_client* message = new message_server_to_client;
		message->receiver_id = client.second.id;
		message->round_time = ROUND_LENGTH - mRoundClock.getElapsedTime().asSeconds();
		message->input_number = client.second.latestReceivedInputSequence;
		message->game_state = GAME_STATE::ROUND_RUNNING;
		message->client_count = (uint8_t)clients.size();
		memcpy(message->client_data, dataArr, sizeof(dataArr));

		mNetworkManager.pushMessage(client.first, message);
	}
}
void GameState::checkTime()
{
	if (ROUND_LENGTH - mRoundClock.getElapsedTime().asSeconds() < 0)
	{
		auto& stack = Context::getInstance().stack;
		stack->clearStates();
		stack->pushState(GAME_STATE::ROUND_END);
	}
}
}