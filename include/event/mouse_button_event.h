#ifndef __LIGHTSPEED_MOUSE_BUTTON_EVENT_H_
#define __LIGHTSPEED_MOUSE_BUTTON_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct MouseButtonEvent final : public entityx::Event<MouseButtonEvent> {
  
  MouseButtonEvent(int button, int action, int mods) :
      button(button),
      action(action),
      mods(mods) {
  }
  
  int button;
  int action;
  int mods;
  
};

}

#endif

