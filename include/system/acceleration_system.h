#ifndef __LIGHTSPEED_ACCELERATION_SYSTEM_H_
#define __LIGHTSPEED_ACCELERATION_SYSTEM_H_

#include <entityx/entityx.h>

namespace lightspeed {

class AccelerationSystem final : public entityx::System<AccelerationSystem> {
  
public:
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
};

}

#endif

