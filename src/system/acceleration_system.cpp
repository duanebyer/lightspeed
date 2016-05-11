#include "system/acceleration_system.h"

#include <cmath>

#include "vector.h"

#include "component/acceleration_component.h"
#include "component/body_component.h"

#include "utility.h"

using namespace lightspeed;

void AccelerationSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
  
  entities.each<BodyComponent, AccelerationComponent>(
    [delta](
        entityx::Entity entity,
        BodyComponent& body,
        AccelerationComponent& acceleration) {
      
      // The momentum of the particle should be increased based on the
      // acceleration that the particle is experiencing.
      body.momentum += delta * acceleration.acceleration;
      
      // Then, the energy should be adjusted so that the energy-momentum
      // relationship is still satisfied.
      body.energy = std::sqrt(
        LIGHT_SPEED * LIGHT_SPEED + body.momentum.normSq());
    });
}

