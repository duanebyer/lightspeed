#ifndef __LIGHTSPEED_MOUSE_POSITION_EVENT_H_
#define __LIGHTSPEED_MOUSE_POSITION_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct MousePositionEvent final : public entityx::Event<MousePositionEvent> {
  
  MousePositionEvent(double mouseX, double mouseY) :
      mouseX(mouseX),
      mouseY(mouseY) {
  }
  
  double mouseX;
  double mouseY;
  
};

}

#endif

