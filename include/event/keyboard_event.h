#ifndef __LIGHTSPEED_KEYBOARD_EVENT_H_
#define __LIGHTSPEED_KEYBOARD_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct KeyboardEvent final : public entityx::Event<KeyboardEvent> {
  
  KeyboardEvent(int key, int scancode, int action, int mods) :
      key(key),
      scancode(scancode),
      action(action),
      mods(mods) {
  }
  
  int key;
  int scancode;
  int action;
  int mods;
  
};

}

#endif

