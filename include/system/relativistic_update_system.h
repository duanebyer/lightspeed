#ifndef __RELATIVISTIC_UPDATE_SYSTEM_H_
#define __RELATIVISTIC_UPDATE_SYSTEM_H_

#include <entityx/entityx.h>

namespace lightspeed {

class RelativisticUpdateSystem final :
    public entityx::System<RelativisticUpdateSystem> {
  
public:
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
};

}

#endif

