#include "system/relativistic_update_system.h"

#include <cmath>

#include "component/body_component.h"
#include "component/player_component.h"

#include "event/relativistic_update_event.h"

#include "utility.h"

using namespace lightspeed;

void RelativisticUpdateSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
  
  entities.each<PlayerComponent, BodyComponent>(
    [delta, &events](
        entityx::Entity entity,
        PlayerComponent& player,
        BodyComponent& body) {
      
      // Calculate gamma and use it to determine the time dilation amount.
      double gamma = (body.energy * body.energy - body.momentum.normSq()) /
                      LIGHT_SPEED / LIGHT_SPEED;
      
      // Emit an event storing the time dilation information.
      events.emit<RelativisticUpdateEvent>(delta, gamma * delta);
    });
}
