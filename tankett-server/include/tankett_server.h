// tankett_server.h

#ifndef TANKETT_SERVER_H_INCLUDED
#define TANKETT_SERVER_H_INCLUDED

#include <new>
#include <memory>
#include <map>
#include <vector>

#include <alpha.h>
#include <tankett_shared.h>

using namespace alpha;

namespace tankett {
   void debugf(const char *format, ...);

   enum client_state
   {
	   Disconnected,
	   Connecting,
	   Connected,
   };

   struct challenge
   {
	   bool operator ==(const challenge& other) const;
	   bool operator ==(const ip_address& addr) const;
	   ip_address addr_;
	   uint64 client_key_;
	   time timestamp_;
   };

   struct client
   {
	   ip_address addr_;
	   client_state state_;
   };

   template<typename T>
   int get_index(std::vector<T> vector, const T& obj)
   {
	   for (int i = 0; i < vector.size(); ++i)
	   {
		   if (vector[i].addr_ == obj.addr_)
			   return i;
	   }
	   return -1;
   }

   struct server {
      server();

      bool init();
      void shut();
      void run();

      void update(const time &dt);
      void receive(const time &dt);
      void send(const time &dt);

      bool running_;
      time send_accumulator_;
      ip_address local_;
	  udp_socket socket_;
	  std::vector<std::unique_ptr<protocol_packet_header>> message_queue_;
	  uint64 key_;
	  std::vector<challenge> challenges_;
	  std::vector<client> clients_;
   };


} // !tankett

#endif // !TANKETT_SERVER_H_INCLUDED
