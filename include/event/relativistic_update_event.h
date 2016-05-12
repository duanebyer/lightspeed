#ifndef __RELATIVISTIC_UPDATE_EVENT_H_
#define __RELATIVISTIC_UPDATE_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct RelativisticUpdateEvent final :
    public entityx::Event<RelativisticUpdateEvent> {
  
  RelativisticUpdateEvent(double deltaPrime, double delta) :
      deltaPrime(deltaPrime),
      delta(delta) {
  }
  
  // The amount of time that has passed in the moving (player's) coordinate
  // frame.
  double deltaPrime;
  // The amount of time that has passed in the "resting" coordinate frame.
  double delta;
};

}

#endif

