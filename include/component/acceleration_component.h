#ifndef __LIGHTSPEED_ACCELERATION_COMPONENT_H_
#define __LIGHTSPEED_ACCELERATION_COMPONENT_H_

#include "vector.h"

namespace lightspeed {

struct AccelerationComponent {
  
  AccelerationComponent() :
      acceleration() {
  }
  
  AccelerationComponent(Vector acceleration) :
      acceleration(acceleration) {
  }
  
  Vector acceleration;
  
};

}

#endif

