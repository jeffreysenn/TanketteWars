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
	uint8 base[2048];
	byte_stream receive_stream(sizeof(base), base);
	ip_address OUT_addr;
	if (!mSocket.recv_from(OUT_addr, receive_stream))
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
			if (!(request.protocol_ == PROTOCOL_ID &&
				request.version_ == PROTOCOL_VERSION))
				break;

			if (mChallenges.find(OUT_addr) != mChallenges.end())
				break;

			Challenge challenge;
			challenge.serverKey = mw::helper::Number::getRandom<uint64>();
			challenge.clientKey = request.client_key_;
			challenge.timestamp = time::now();

			mChallenges.insert(std::make_pair(OUT_addr, challenge));
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
		auto found = mChallenges.find(OUT_addr);
		if (found == mChallenges.end())
			break;

		auto& challenge = found->second;
		if (challenge_response.combined_key_ == (challenge.clientKey ^ challenge.serverKey))
		{
			if (mClients.find(OUT_addr) != mClients.end())
				break;

			Client client;
			client.id = (uint8)mClients.size();
			client.serverKey = challenge.serverKey;
			client.clientKey = challenge.clientKey;
			client.xorinator = crypt::xorinator(client.serverKey, client.clientKey);
			client.connectionTime = time::now();
			client.latestReceiveTime = time::now();

			mClients.insert(std::make_pair(OUT_addr, client));
			mChallenges.erase(OUT_addr);
			debugf("[nfo] a client has connected");
			debugf("[nfo] IP: %s, SK: %#08x, CK: %#08x", OUT_addr.as_string(), client.serverKey, client.clientKey);
		}

	} break;

	case tankett::PACKET_TYPE_PAYLOAD:
	{
		if (mClients.find(OUT_addr) == mClients.end())
			break;

		protocol_payload packet;
		if (!packet.read(reader))
		{
			debugf("[err] IP: %s fail to read payload", OUT_addr.as_string());
			break;
		}

		mClients[OUT_addr].xorinator.decrypt(packet.length_, packet.payload_);

		byte_stream stream(packet.length_, packet.payload_);
		byte_stream_reader payload_reader(stream);

		bool shouldRead = true;
		while (!payload_reader.eos() && shouldRead)
		{
			network_message_type message_type = (network_message_type)payload_reader.peek();
			switch (message_type)
			{
			case tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER:
			{
				message_client_to_server* message_c2s = new message_client_to_server;
				if (!message_c2s->read(payload_reader))
				{
					// error
				}
				mClients[OUT_addr].receivedMessages.push_back(message_c2s);
			} break;

			default:
				shouldRead = false;
				break;
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

void NetworkManager::clearAllClientsReceivedMessages()
{
	for (auto& client : mClients)
	{
		client.second.clear_received_messages();
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