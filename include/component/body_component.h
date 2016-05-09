#ifndef __LIGHTSPEED_BODY_COMPONENT_H_
#define __LIGHTSPEED_BODY_COMPONENT_H_

#include "quaternion.h"
#include "vector.h"

namespace lightspeed {

struct BodyComponent final {
  
  BodyComponent() :
      position(),
      rotation(Quaternion(1.0, Vector())),
      velocity() {
  }
  
  BodyComponent(Vector position, Quaternion rotation, Vector velocity) :
      position(position),
      rotation(rotation),
      velocity(velocity) {
  }
  
  Vector position;
  Quaternion rotation;
  Vector velocity;
  
};

}

#endif

