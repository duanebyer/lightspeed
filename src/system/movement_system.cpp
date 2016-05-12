#include "system/movement_system.h"

#include "vector.h"

#include "component/body_component.h"

#include "event/relativistic_update_event.h"

using namespace lightspeed;

void MovementSystem::configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) {
  
  m_entities = &entities;
  m_events = &events;
  
  events.subscribe<RelativisticUpdateEvent>(*this);
}

void MovementSystem::receive(RelativisticUpdateEvent const& event) {
  
  m_entities->each<BodyComponent>(
    [event](entityx::Entity entity, BodyComponent& body) {
      body.position +=
        event.deltaPrime * body.momentum * LIGHT_SPEED / body.energy;
  });
}

void MovementSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
}

