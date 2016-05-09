#ifndef __LIGHTSPEED_MOVEMENT_SYSTEM_H_
#define __LIGHTSPEED_MOVEMENT_SYSTEM_H_

#include <entityx/entityx.h>

namespace lightspeed {

class MovementSystem final : public entityx::System<MovementSystem> {
  
public:
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
};

}

#endif

