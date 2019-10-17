// tankett_server.cc

#include "tankett_server.h"

namespace tankett {
   server::server()
      : running_(false)
	   , key_(666666)
   {
   }

   bool server::init() {
      if (!network_init()) {
         debugf("[err] could not initialize network!");
         return false;
      }

      dynamic_array<ip_address> addressess;
      if (!ip_address::local_addresses(addressess)) {
         debugf("[err] could not retreive local ip addressess!");
         return false;
      }

      debugf("[nfo] found %d ip addressess", (int)addressess.size());

      for (int index = 0; index < (int)addressess.size(); index++) {
         debugf("[nfo]   #%d - %s", index, addressess[index].as_string());
      }

      local_ = addressess[0];
      local_.set_port(PROTOCOL_PORT);

	  if (!socket_.open(local_))
		  return false;

      running_ = true;

      return running_;
   }

   void server::shut() {
	   socket_.close();
      network_shut();
   }

   void server::run() {
      debugf("[nfo] server running at %s", local_.as_string());
      const time start = time::now();
      time current = time::now();
      while (running_) {
         time now = time::now();
         time dt = now - current;
         current = now;

         receive(dt);
         update(dt);
         send(dt);
      }
   }

   void server::update(const time &dt) {
   }

   void server::receive(const time &dt) {
	   uint8 base[2048];
	   byte_stream receive_stream(sizeof(base), base);
	   ip_address OUT_addr;
	   if (!socket_.recv_from(OUT_addr, receive_stream))
		   return;

	   byte_stream_reader reader(receive_stream);
	   const packet_type type = (packet_type) reader.peek();

	   switch (type)
	   {
	   case tankett::PACKET_TYPE_CONNECTION_REQUEST:
	   {
		   protocol_connection_request request;
		   if (request.serialize(reader))
		   {
			   debugf("IP: %s, CK: %d", OUT_addr.as_string(), request.client_key_);

			   if (!(request.protocol_ == PROTOCOL_ID &&
				   request.version_ == PROTOCOL_VERSION))
				   break;

			   challenge challenge;
			   challenge.addr_ = OUT_addr;
			   challenge.client_key_ = request.client_key_;
			   challenge.timestamp_ = time::now();

			   bool can_push = true;
			   for (const auto& cha : challenges_)
			   {
				   if (cha == challenge)
					   can_push = false;
			   }

			   if (can_push)
				   challenges_.push_back(challenge);
		   }
	   } break;
	   case tankett::PACKET_TYPE_CHALLENGE_RESPONSE:
	   {
		   protocol_challenge_response challenge_response;
		   if (challenge_response.serialize(reader))
		   {
			   for (auto& challenge : challenges_)
			   {
				   if (challenge == OUT_addr)
				   {
					   if (challenge_response.combined_key_ == (key_ ^ challenge.client_key_))
					   {
						   bool client_exists = false;
						   for (auto& client : clients_)
						   {
							   if (client.addr_ == OUT_addr)
								   client_exists = true;
						   }

						   if (!client_exists)
						   {
							   client new_client;
							   new_client.addr_ = OUT_addr;
							   new_client.state_ = Connected;
							   clients_.push_back(new_client);
						   }

						   challenges_.erase(challenges_.begin() + get_index(challenges_, challenge));
					   }
				   }
			   }
		   }
	   } break;
	   case tankett::PACKET_TYPE_KEEP_ALIVE:
		   break;
	   case tankett::PACKET_TYPE_PAYLOAD:
		   break;
	   case tankett::PACKET_TYPE_DISCONNECT:
		   break;
	   default:
		   break;
	   }
	   

   }

   void server::send(const time &dt) {
	   uint8 send_arr[2048];
	   byte_stream stream(sizeof(send_arr), send_arr);

      send_accumulator_ += dt;
      if (send_accumulator_ > time(100)) {
         send_accumulator_ -= time(100);

		 for (auto& challenge : challenges_)
		 {
			 stream.reset();
			byte_stream_writer writer(stream);
			 // if...

			 protocol_connection_challenge challenge_message(key_);
			 challenge_message.serialize(writer);
			 if (socket_.send_to(challenge.addr_, stream))
			 {
				 // ...
			 }
		 }

		 for (auto& client : clients_)
		 {
			 stream.reset();
			 byte_stream_writer writer(stream);

			 protocol_payload payload;
			 //byte_stream s(sizeof(payload.payload_), payload.payload_);
			 //byte_stream_writer w(s);
			 //
			 //network_message_ping message;
			 //message.serialize(w);
			 payload.sequence_ = 99999;
			 if (payload.serialize(writer))
			 {
				 if (!socket_.send_to(client.addr_, stream))
				 {
					 // error..
				 }
			 }
		 }


      }
   }
   bool challenge::operator==(const challenge& other) const
   {
	   return addr_ == other.addr_;
   }
   bool challenge::operator==(const ip_address& addr) const
   {
	   return addr_ == addr;
   }
} // !tankett

