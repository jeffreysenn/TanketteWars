// tankett_server.cc

#include "tankett_debug.h"
#include "NetworkManager.h"
#include "Helpers/helper.h"
namespace server
{

NetworkManager::NetworkManager()
{
	if (init())
		debugf("[nfo] server running at %s", mlocal.as_string());
	else
		debugf("[err] could not initialize server!");
}

bool NetworkManager::init()
{
	if (!network_init())
	{
		debugf("[err] could not initialize network!");
		return false;
	}

	mlocal = getLocalAddr();
	mlocal.set_port(PROTOCOL_PORT);

	if (!mSocket.open(mlocal))
		return false;

	return true;
}

void NetworkManager::shut()
{
	mSocket.close();
	network_shut();
}

void NetworkManager::receive()
{
	checkClinetConnection();

	uint8 base[2048];
	byte_stream receive_stream(sizeof(base), base);
	ip_address outAddr;
	if (!mSocket.recv_from(outAddr, receive_stream))
		return;

	byte_stream_reader reader(receive_stream);
	const packet_type type = (packet_type)reader.peek();

	switch (type)
	{
	case tankett::PACKET_TYPE_CONNECTION_REQUEST:
	{
		protocol_connection_request request;
		if (request.serialize(reader))
		{
			if (mClients.size() >= 4)
				break;

			if (!(request.protocol_ == PROTOCOL_ID &&
				request.version_ == PROTOCOL_VERSION))
				break;

			if (mChallenges.find(outAddr) != mChallenges.end())
				break;

			Challenge challenge;
			challenge.serverKey = mw::helper::Number::getRandom<uint64>();
			challenge.clientKey = request.client_key_;
			challenge.timestamp = time::now();

			mChallenges.insert(std::make_pair(outAddr, challenge));
		}
	} break;

	case tankett::PACKET_TYPE_CHALLENGE_RESPONSE:
	{
		protocol_challenge_response challenge_response;
		if (!challenge_response.serialize(reader))
		{
			//error
			break;
		}
		auto found = mChallenges.find(outAddr);
		if (found == mChallenges.end())
			break;

		auto& challenge = found->second;
		if (challenge_response.combined_key_ == (challenge.clientKey ^ challenge.serverKey))
		{
			if (mClients.find(outAddr) != mClients.end())
				break;

			Client client;
			uint8 id = 0;
			for (auto& existingClient : mClients)
			{
				if (id == existingClient.second.id)
					++id;
			}
			client.id = id;
			client.serverKey = challenge.serverKey;
			client.clientKey = challenge.clientKey;
			client.xorinator = crypt::xorinator(client.serverKey, client.clientKey);
			client.connectionTime = time::now();
			client.latestReceiveTime = time::now();

			mClients.insert(std::make_pair(outAddr, client));
			mChallenges.erase(outAddr);
			debugf("[nfo] a client has connected");
			debugf("[nfo] IP: %s, SK: %#08x, CK: %#08x", outAddr.as_string(), client.serverKey, client.clientKey);
		}

	} break;

	case tankett::PACKET_TYPE_PAYLOAD:
	{
		if (mClients.find(outAddr) == mClients.end())
			break;


		protocol_payload packet;
		if (!packet.read(reader))
		{
			debugf("[err] IP: %s fail to read payload", outAddr.as_string());
			break;
		}

		if (!packet.is_newer(mClients[outAddr].latestReceivedSequence)) break;

		mClients[outAddr].latestReceivedSequence = packet.sequence_;
		mClients[outAddr].latestReceiveTime = time::now();
		mClients[outAddr].xorinator.decrypt(packet.length_, packet.payload_);

		byte_stream stream(packet.length_, packet.payload_);
		byte_stream_reader payload_reader(stream);

		::std::vector<message_client_to_server> inputMsgs;

		bool shouldRead = true;
		while (!payload_reader.eos() && shouldRead)
		{
			network_message_type message_type = (network_message_type)payload_reader.peek();
			switch (message_type)
			{
			case NETWORK_MESSAGE_PING:
			{
				network_message_ping messagePing;
				if (!messagePing.read(payload_reader))
				{
					// error
				}
				auto& pingMap = mClients[outAddr].pingMap;
				auto found = pingMap.find(messagePing.sequence_);
				if (found != pingMap.end())
				{
					mClients[outAddr].ping = (uint32)(time::now() - found->second).tick_;
					pingMap.erase(pingMap.begin(), found);
				}
			} break;

			case tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER:
			{
				message_client_to_server messageC2S;
				if (!messageC2S.read(payload_reader))
				{
					// error
				}
				// check input sequence
				// only push back messages that contains newer input sequence
				if (messageC2S.input_number > mClients[outAddr].latestReceivedInputSequence)
				{
					inputMsgs.push_back(messageC2S);
				}
			} break;

			default:
				shouldRead = false;
				break;
			}
		}

		::std::sort(inputMsgs.begin(), inputMsgs.end());
		for (const auto& inputMsg : inputMsgs)
		{
			if (inputMsg.input_number > mClients[outAddr].latestReceivedInputSequence)
			{
				mClients[outAddr].latestReceivedInputSequence = inputMsg.input_number;
				mClients[outAddr].receivedMessages.push_back(new message_client_to_server(inputMsg));
			}
		}
	} break;
	}
}

void NetworkManager::send()
{
	for (auto& challenge : mChallenges)
	{
		protocol_connection_challenge challenge_message(challenge.second.serverKey);
		if (!sendPackage(mSocket, challenge.first, challenge_message))
		{
			// error
		}
	}

	processClientQueues();
}

void NetworkManager::pushMessage(const ip_address& clientAddr, network_message_header* message)
{
	auto found = mClients.find(clientAddr);
	if (found == mClients.end())
	{
		debugf("[err] could not find client!");
		return;
	}
	found->second.sendMessageQueue.push_back(message);
}

void NetworkManager::processClientQueues()
{
	// note: use the same server sequence number for all sends
	const uint32 current_sequence_number = mServerSequence++;
	protocol_payload packet(current_sequence_number);

	// note: iterate over all clients and pack ping messages into payload
	for (auto& client : mClients)
	{
		network_message_ping* pingMsg = new network_message_ping(mPingSequence);
		client.second.pingMap[mPingSequence] = time::now();
		pushMessage(client.first, pingMsg);
		++mPingSequence;
	}

	// note: iterate over all clients and pack messages into payload, then send
	for (auto& client : mClients)
	{
		auto& messages = client.second.sendMessageQueue;

		// note: calculate the number of messages we can pack into the payload
		int messages_evaluated = 0;
		byte_stream stream(sizeof(packet.payload_), packet.payload_);
		byte_stream_evaluator evaluator(stream);
		for (auto iter = messages.begin(); iter != messages.end(); ++iter)
		{
			if (!(*iter)->write(evaluator))
			{
				break;
			}

			messages_evaluated++;
		}

		// note: actual packing of messages into the payload
		int messages_packed = 0;
		byte_stream_writer writer(stream);
		for (auto iter = messages.begin(); iter != messages.end(); ++iter)
		{
			if (!(*iter)->write(writer))
			{
				break;
			}

			messages_packed++;
			if (messages_evaluated == messages_packed)
			{
				break;
			}
		}

		// note: did the packing succeed?
		if (messages_evaluated != messages_packed)
		{
			debugf("[err] %s - sequence: %u - messages_evaluated != messages_packed",
				   current_sequence_number,
				   client.first.as_string());
			continue;
		}

		// note: finalize packet by setting the payload length
		packet.length_ = (uint16)stream.length();

		// note: ... then we encrypt it!
		client.second.xorinator.encrypt(packet.length_, packet.payload_);

		// note: send payload to client
		if (sendPackage(mSocket, client.first, packet))
		{
			// note: if sending succeeds we can:
			//       - delete messages sent
			//       - remove them from client message queue
			for (int remove_message_index = 0;
				 remove_message_index < messages_packed;
				 remove_message_index++)
			{
				delete messages[remove_message_index];
			}

			messages.erase(messages.begin(), messages.begin() + messages_packed);
		}
	}
}

bool NetworkManager::allClientReceivedMessagesEmpty()
{
	for (auto& client : mClients)
	{
		if (!client.second.receivedMessages.empty())
			return false;
	}
	return true;
}

void NetworkManager::clearAllClientsReceivedMessages()
{
	for (auto& client : mClients)
	{
		client.second.clear_received_messages();
	}
}

void NetworkManager::resetClientsLatestReceivedInputSequence()
{
	for (auto& client : mClients)
	{
		client.second.latestReceivedInputSequence = 0;
	}
}

constexpr float NO_REC_KICK_TIME = 2.f;
void NetworkManager::checkClinetConnection()
{
	for (auto it = mClients.begin(); it != mClients.end();)
	{
		if ((::alpha::time::now() - it->second.latestReceiveTime).as_seconds() > NO_REC_KICK_TIME)
		{
			it = mClients.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void NetworkManager::Client::clear_received_messages()
{
	for (auto& received_message : receivedMessages)
	{
		delete received_message;
	}
	receivedMessages.clear();
}

}