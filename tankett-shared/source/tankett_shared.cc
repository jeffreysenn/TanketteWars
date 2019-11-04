// tankett_shared.cc

#include "tankett_shared.h"
#include "tankett_debug.h"

namespace tankett
{
protocol_packet_header::protocol_packet_header()
	: type_(0xff)
{
}

protocol_packet_header::protocol_packet_header(packet_type type)
	: type_((uint8)type)
{
}

protocol_connection_request::protocol_connection_request()
	: protocol_packet_header(PACKET_TYPE_CONNECTION_REQUEST)
	, protocol_(PROTOCOL_ID)
	, version_(PROTOCOL_VERSION)
	, client_key_(0)
	, padding_{}
{
}

protocol_connection_request::protocol_connection_request(uint64 client_key)
	: protocol_packet_header(PACKET_TYPE_CONNECTION_REQUEST)
	, protocol_(PROTOCOL_ID)
	, version_(PROTOCOL_VERSION)
	, client_key_(client_key)
	, padding_{}
{
}

protocol_connection_denied::protocol_connection_denied()
	: protocol_packet_header(PACKET_TYPE_CONNECTION_DENIED)
	, reason_(0)
{
}

protocol_connection_denied::protocol_connection_denied(denied_reason_type reason)
	: protocol_packet_header(PACKET_TYPE_CONNECTION_DENIED)
	, reason_((uint8)reason)
{
}

protocol_connection_challenge::protocol_connection_challenge()
	: protocol_packet_header(PACKET_TYPE_CONNECTION_CHALLENGE)
	, server_key_(0)
{
}

protocol_connection_challenge::protocol_connection_challenge(uint64 server_key)
	: protocol_packet_header(PACKET_TYPE_CONNECTION_CHALLENGE)
	, server_key_(server_key)
{
}

protocol_challenge_response::protocol_challenge_response()
	: protocol_packet_header(PACKET_TYPE_CHALLENGE_RESPONSE)
	, combined_key_(0)
	, padding_{}
{
}

protocol_challenge_response::protocol_challenge_response(uint64 combined_key)
	: protocol_packet_header(PACKET_TYPE_CHALLENGE_RESPONSE)
	, combined_key_(combined_key)
	, padding_{}
{
}

protocol_keep_alive::protocol_keep_alive()
	: protocol_packet_header(PACKET_TYPE_KEEP_ALIVE)
{
}

protocol_payload::protocol_payload()
	: protocol_packet_header(PACKET_TYPE_PAYLOAD)
	, sequence_(0)
	, length_(0)
	, payload_{}
{
}

protocol_payload::protocol_payload(uint32 sequence)
	: protocol_packet_header(PACKET_TYPE_PAYLOAD)
	, sequence_(sequence)
	, length_(0)
	, payload_{}
{
}

bool protocol_payload::is_newer(const uint32 sequence)
{
	// note: evening reading matter
	//       https://www.ietf.org/rfc/rfc1323.txt
	return ((sequence_ > sequence) && (sequence_ - sequence <= 2147483647)) ||
		((sequence_ < sequence) && (sequence - sequence_ > 2147483647));
}

protocol_disconnect::protocol_disconnect()
	: protocol_packet_header(PACKET_TYPE_DISCONNECT)
	, client_key_(0)
{
}

protocol_disconnect::protocol_disconnect(uint64 client_key)
	: protocol_packet_header(PACKET_TYPE_DISCONNECT)
	, client_key_(client_key)
{
}

network_message_header::network_message_header()
	: type_(0xff)
{
}

network_message_header::network_message_header(network_message_type type)
	: type_((uint8)type)
{
}

network_message_ping::network_message_ping()
	: network_message_header(NETWORK_MESSAGE_PING)
	, sequence_(0u)
{
}

network_message_ping::network_message_ping(uint8 sequence)
	: network_message_header(NETWORK_MESSAGE_PING)
	, sequence_(sequence)
{
}

bool network_message_ping::is_newer(uint8 sequence)
{
	return ((sequence_ > sequence) && (sequence_ - sequence <= 127)) ||
		((sequence_ < sequence) && (sequence - sequence_ > 127));
}

ip_address getLocalAddr()
{
	dynamic_array<ip_address> addressess;
	if (!ip_address::local_addresses(addressess))
	{
		debugf("[err] could not retreive local ip addressess!");
		return ip_address();
	}

	debugf("[nfo] found %d ip addressess", (int)addressess.size());

	for (int index = 0; index < (int)addressess.size(); index++)
	{
		debugf("[nfo]   #%d - %s", index, addressess[index].as_string());
	}

	return addressess[0];
}

bool PlayerState::operator==(const PlayerState& rhs) const
{
	if (alive)
	{
		if (!(
			alive == rhs.alive
			&& ::std::abs(position.x_ - rhs.position.x_) < 0.01f
			&& ::std::abs(position.y_ - rhs.position.y_) < 0.01f
			&& bullet_count == rhs.bullet_count
			))
			return false;
	}
	else
	{
		if (!(
			alive == rhs.alive
			&& bullet_count == rhs.bullet_count
			))
			return false;
	}

	for (int i = 0; i < bullet_count; ++i)
	{
		bool foundSameID = false;
		for (int j = 0; j < bullet_count; ++j)
		{
			if(bullets[i].id == rhs.bullets[j].id)
			{
				if (bullets[i] != rhs.bullets[j])
				{
					return false;
				}
				foundSameID = true;
			}
		}
		if (!foundSameID) 
			return false;
	}

	return true;
}

bool bullet_data::operator==(const bullet_data& rhs) const
{
	return ::std::abs(position.x_ - rhs.position.x_) < 0.01f
		&& ::std::abs(position.y_ - rhs.position.y_) < 0.01f;
}

bool bullet_data::operator!=(const bullet_data& rhs) const
{
	return !(*this == rhs);
}
} // !alpha


