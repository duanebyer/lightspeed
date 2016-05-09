#include "system/movement_system.h"

#include "vector.h"

#include "component/body_component.h"

using namespace lightspeed;

void MovementSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
  
  entities.each<BodyComponent>(
    [delta](entityx::Entity entity, BodyComponent& body) {
      body.position += body.velocity;
  });
}

