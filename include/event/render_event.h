#ifndef __LIGHTSPEED_RENDER_EVENT_H_
#define __LIGHTSPEED_RENDER_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct RenderEvent final : public entityx::Event<RenderEvent> {
  
  RenderEvent(int viewportWidth, int viewportHeight) :
      viewportWidth(viewportWidth),
      viewportHeight(viewportHeight) {
  }
  
  int viewportWidth;
  int viewportHeight;
  
};

}

#endif

