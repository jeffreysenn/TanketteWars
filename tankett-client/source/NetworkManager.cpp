#include "NetworkManager.h"
#include "..\include\NetworkManager.h"
#include "tankett_debug.h"
#include "Helpers/Helper.h"
namespace client
{

NetworkManager::NetworkManager()
	: mClientKey(::mw::helper::Number::getRandom<uint64>())
	, mServerKey(0u)
	, mState(ConnectionState::None)
	, mClientSequence(0u)
	, mServerSequence(0u)
{
	if (!network_init())
	{
		::tankett::debugf("[err] could not initialize network!");
	}
	if (!mSocket.open())
	{
		::tankett::debugf("[err] could not open socket!");
	}

	mLocalAddr = ::tankett::getLocalAddr();
}

NetworkManager::~NetworkManager()
{
	mSocket.close();
	network_shut();
}

void NetworkManager::send()
{
	switch (mState)
	{
	case NetworkManager::ConnectionState::Discovering:
	{
		uint32 broadcastHost = mLocalAddr.host_ | 0x000000ffu;
		mServerAddr = ip_address(broadcastHost, ::tankett::PROTOCOL_PORT);

		::tankett::protocol_connection_request connectionRequest;
		connectionRequest.client_key_ = mClientKey;
		connectionRequest.protocol_ = ::tankett::PROTOCOL_ID;
		connectionRequest.version_ = ::tankett::PROTOCOL_VERSION;

		if (!sendPackage(mSocket, mServerAddr, connectionRequest))
		{
			::tankett::debugf("[err] fail to send!");
		}
	} break;
	case NetworkManager::ConnectionState::Challenging:
	{
		::tankett::protocol_challenge_response challengeResponse(mClientKey ^ mServerKey);
		if (!sendPackage(mSocket, mServerAddr, challengeResponse))
		{
			::tankett::debugf("[err] fail to send!");
		}
	} break;
	case NetworkManager::ConnectionState::Connected:
	{
		processMessages();
	} break;
	}
}

void NetworkManager::receive()
{
	uint8 receiveArr[2048];
	byte_stream receiveStream(sizeof(receiveArr), receiveArr);
	ip_address outAddr;
	if (!mSocket.recv_from(outAddr, receiveStream))
		return;

	byte_stream_reader reader(receiveStream);
	::tankett::packet_type type = (::tankett::packet_type)reader.peek();

	switch (type)
	{
	case tankett::PACKET_TYPE_CONNECTION_DENIED:
		break;
	case tankett::PACKET_TYPE_CONNECTION_CHALLENGE:
	{
		if (mState != ConnectionState::Discovering) break;
		::tankett::protocol_connection_challenge connectionChallenge;
		connectionChallenge.serialize(reader);
		mServerAddr = outAddr;
		mServerKey = connectionChallenge.server_key_;
		mXorinator = crypt::xorinator(mServerKey, mClientKey);
		mState = ConnectionState::Challenging;
	} break;
	case tankett::PACKET_TYPE_CHALLENGE_RESPONSE:
		break;
	case tankett::PACKET_TYPE_KEEP_ALIVE:
		break;
	case tankett::PACKET_TYPE_PAYLOAD:
	{
		if (mState != ConnectionState::Connected)
		{
			::tankett::debugf("[nfo] connected to server");
			::tankett::debugf("[nfo] IP: %s, SK: %#08x, CK: %#08x", outAddr.as_string(), mServerKey, mClientKey);
			mState = ConnectionState::Connected;
		}

		::tankett::protocol_payload packet;
		if (!packet.read(reader))
		{
			::tankett::debugf("[err] IP: %s fail to read payload", outAddr.as_string());
			break;
		}

		if (!packet.is_newer(mServerSequence)) break;

		mServerSequence = packet.sequence_;

		mXorinator.decrypt(packet.length_, packet.payload_);

		byte_stream payloadStream(packet.length_, packet.payload_);
		byte_stream_reader payloadReader(payloadStream);

		bool shouldRead = true;
		while (!payloadReader.eos() && shouldRead)
		{
			::tankett::network_message_type messageType = (::tankett::network_message_type)payloadReader.peek();
			switch (messageType)
			{
			case tankett::NETWORK_MESSAGE_PING:
			{
				std::unique_ptr<::tankett::network_message_ping> messagePing(new ::tankett::network_message_ping);
				if (!messagePing->read(payloadReader))
				{
					// error
				}
				// no need to push into received buffer
				mSendMessageQueue.push_back(std::move(messagePing));
			} break;
			case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
			{
				std::unique_ptr<::tankett::message_server_to_client> messageS2C(new ::tankett::message_server_to_client);
				if (!messageS2C->read(payloadReader))
				{
					// error
				}
				mReceivedMessages.push_back(std::move(messageS2C));
			} break;
			default:
				shouldRead = false;
				break;
			}
		}
	} break;
	case tankett::PACKET_TYPE_DISCONNECT:
		break;
	default:
		break;
	}

}

void NetworkManager::pushMessage(::std::unique_ptr<::tankett::network_message_header> message)
{
	mSendMessageQueue.push_back(::std::move(message));
}

void NetworkManager::clearReceivedMessages()
{
	mReceivedMessages.clear();
}

void NetworkManager::processMessages()
{
	const uint32 current_sequence_number = mClientSequence++;
	::tankett::protocol_payload packet(current_sequence_number);

	// note:  pack messages into payload, then send
	// note: calculate the number of messages we can pack into the payload
	int messages_evaluated = 0;
	byte_stream stream(sizeof(packet.payload_), packet.payload_);
	byte_stream_evaluator evaluator(stream);
	for (auto iter = mSendMessageQueue.begin(); iter != mSendMessageQueue.end(); ++iter)
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
	for (auto iter = mSendMessageQueue.begin(); iter != mSendMessageQueue.end(); ++iter)
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
		::tankett::debugf("[err] sequence: %u - messages_evaluated != messages_packed",
			   current_sequence_number);
	}

	// note: finalize packet by setting the payload length
	packet.length_ = (uint16)stream.length();

	// note: ... then we encrypt it!
	mXorinator.encrypt(packet.length_, packet.payload_);

	// note: send payload to client
	if (sendPackage(mSocket, mServerAddr, packet))
	{
		// note: if sending succeeds we can:
		//       - remove them from client message queue
		//       - messages sent will be delete because of unique_ptr
		mSendMessageQueue.erase(mSendMessageQueue.begin(), mSendMessageQueue.begin() + messages_packed);
	}

}

}