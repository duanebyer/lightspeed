#ifndef __LIGHTSPEED_BODY_COMPONENT_H_
#define __LIGHTSPEED_BODY_COMPONENT_H_

#include <cmath>

#include "quaternion.h"
#include "utility.h"
#include "vector.h"

namespace lightspeed {

struct BodyComponent final {
  
  BodyComponent() :
      position(),
      rotation(Quaternion(1.0, Vector())),
      momentum(),
      energy(LIGHT_SPEED) {
  }
  
  BodyComponent(
        Vector position,
        Quaternion rotation,
        Vector momentum) :
      position(position),
      rotation(rotation),
      momentum(momentum),
      energy(std::sqrt(LIGHT_SPEED * LIGHT_SPEED + momentum.normSq())) {
  }
  
  Vector position;
  Quaternion rotation;
  Vector momentum;
  double energy;
  
};

}

#endif

