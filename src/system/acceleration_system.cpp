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
      
      // First, divide the velocity into components parallel and perpendicular
      // to the acceleration.
      Vector accelerationDir = acceleration.acceleration.unit();
      Vector beta = body.velocity / LIGHT_SPEED;
      Vector betaParallel = beta.dot(accelerationDir) * accelerationDir;
      Vector betaPerpendicular = beta - betaParallel;
      
      // Calculate gamma as well as the perpendicular gamma factor.
      double gamma = 1.0 / std::sqrt(1.0 - beta.normSq());
      double gammaPerpendicular =
        1.0 / std::sqrt(1.0 - betaPerpendicular.normSq());
      
      // Calculate the change in the velocity.
      Vector deltaV =
        delta *
        (gammaPerpendicular / (gamma * gamma * gamma)) *
        acceleration.acceleration;
      body.velocity += deltaV;
    });
}

