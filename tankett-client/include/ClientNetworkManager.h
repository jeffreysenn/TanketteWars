#pragma once
#include "tankett_shared.h"

#include <vector>
#include <memory>

using namespace tankett;
using namespace alpha;

class ClientNetworkManager
{
public:
	enum class ConnectionState
	{
		None,
		Discovering,
		Challenging,
		Connected,
	};

public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void send();
	void receive();

	void setState(const ConnectionState& state) { mState = state; }
	ConnectionState getState() { return mState; }
	
	std::vector<std::unique_ptr<network_message_header>>& getReceivedMessages() { return mReceivedMessages; }
private:
	void processMessages();

private:
	ConnectionState mState;
	uint64_t mClientKey;
	uint64_t mServerKey;
	ip_address mServerAddr;
	udp_socket mSocket;
	ip_address mLocalAddr;
	std::vector<std::unique_ptr<network_message_header>> mMessageQueue;
	std::vector<std::unique_ptr<network_message_header>> mReceivedMessages;
	crypt::xorinator mXorinator;
	uint32 mClientSequence;
};

