#include "system/acceleration_system.h"

#include <cmath>

#include "vector.h"

#include "component/acceleration_component.h"
#include "component/body_component.h"

#include "event/relativistic_update_event.h"

#include "utility.h"

using namespace lightspeed;

void AccelerationSystem::configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) {
  
  m_entities = &entities;
  m_events = &events;
  
  events.subscribe<RelativisticUpdateEvent>(*this);
}

void AccelerationSystem::receive(RelativisticUpdateEvent const& event) {
  
  m_entities->each<BodyComponent, AccelerationComponent>(
    [event](
        entityx::Entity entity,
        BodyComponent& body,
        AccelerationComponent& acceleration) {
      
      // The momentum of the particle should be increased based on the
      // acceleration that the particle is experiencing.
      body.momentum += event.deltaPrime * acceleration.acceleration;
      
      // Then, the energy should be adjusted so that the energy-momentum
      // relationship is still satisfied.
      body.energy = std::sqrt(
        LIGHT_SPEED * LIGHT_SPEED + body.momentum.normSq());
    });
}

void AccelerationSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
}

