// tankett_server.h

#ifndef TANKETT_SERVER_H_INCLUDED
#define TANKETT_SERVER_H_INCLUDED

#include <memory>
#include <map>
#include <vector>

#include <alpha.h>
#include <tankett_shared.h>

using namespace alpha;
using namespace tankett;

namespace server
{

class NetworkManager
{
public:
	struct Challenge
	{
		uint64 serverKey{};
		uint64 clientKey{};
		alpha::time timestamp;
	};

	struct Client
	{
		void clear_received_messages();

		uint8 id;
		alpha::time connectionTime;
		uint64 serverKey{};
		uint64 clientKey{};
		crypt::xorinator xorinator;
		uint32 latestReceivedSequence{};
		alpha::time latestReceiveTime;

		dynamic_array<network_message_header*> sendMessageQueue;
		dynamic_array<network_message_header*> receivedMessages;
	};

public:
	NetworkManager();

	bool init();
	void shut();

	void receive();
	void send();

	void pushMessage(const ip_address& clientAddr, network_message_header* message);
	void processClientQueues();

	std::map<ip_address, Client>& getClients() { return mClients; }

	void clearAllClientsReceivedMessages();

private:
	ip_address mlocal;
	udp_socket mSocket;

	std::map<ip_address, Challenge> mChallenges;
	std::map<ip_address, Client> mClients;
	uint32 mServerSequence{};

};

#endif // !TANKETT_SERVER_H_INCLUDED

}