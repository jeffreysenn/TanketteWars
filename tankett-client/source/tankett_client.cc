// tankett_client.cc

#include "tankett_client.h"

namespace alpha {
   application *application::create(int &width, int &height, string &title) {
      title = "tankett-wars-client";
      return new tankett::client_app;
   }
} // !alpha

namespace tankett {
   client_app::client_app()
   {
   }

   bool client_app::enter() {
      current_ = time::now();

      text_.set_text("IP: ...");
      transform_.set_position({ 2, 2 });

      return true;
   }

   void client_app::exit() {

   }

   bool client_app::tick() {
      // note: deltatime
      const time now = time::now();
      time dt = now - current_;
      current_ = now;

      // note: quit
      if (keyboard_.is_pressed(KEYCODE_ESCAPE)) {
         return false;
      }

      // note: connect
      if (keyboard_.is_pressed(KEYCODE_C)) {
      }

      // note: disconnect
      if (keyboard_.is_down(KEYCODE_D)) {
      }

      // note: rendering
      render_system_.clear(0xff334455);
      render_system_.render(text_, transform_);

      return true;
   }
} // !tankett
