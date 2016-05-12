#ifndef __LIGHTSPEED_MOVEMENT_SYSTEM_H_
#define __LIGHTSPEED_MOVEMENT_SYSTEM_H_

#include <entityx/entityx.h>

#include "event/relativistic_update_event.h"

namespace lightspeed {

class MovementSystem final : public entityx::System<MovementSystem>,
                             public entityx::Receiver<MovementSystem> {
  
public:
  
  void configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) override;
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
  void receive(RelativisticUpdateEvent const& event);
  
private:
  
  entityx::EntityManager* m_entities;
  entityx::EventManager* m_events;
  
};

}

#endif

