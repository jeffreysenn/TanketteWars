// tankett_client.h

#ifndef TANKETT_CLIENT_H_INCLUDED
#define TANKETT_CLIENT_H_INCLUDED

#include <alpha.h>
#include <tankett_shared.h>

using namespace alpha;

namespace tankett {
   struct client_app : application {
      client_app();

      virtual bool enter() final;
      virtual void exit() final;
      virtual bool tick() final;

      time current_;
      text text_;
      transform transform_;
   };
} // !tankett

#endif // !TANKETT_CLIENT_H_INCLUDED
