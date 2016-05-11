#include "system/player_system.h"

#include <cmath>

#include <entityx/entityx.h>

#include "internal/opengl.h"

#include "component/acceleration_component.h"
#include "component/body_component.h"
#include "component/player_component.h"

using namespace lightspeed;

void PlayerSystem::configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) {
  
  // Store internal references to the entity and event managers.
  m_entities = &entities;
  m_events = &events;
  
  // Subscribe to all of the events.
  events.subscribe<KeyboardEvent>(*this);
  events.subscribe<MouseButtonEvent>(*this);
  events.subscribe<MousePositionEvent>(*this);
}

void PlayerSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
  
  // Find every entity that has a player component and a body component, and
  // apply the correct rotation to it.
  entities.each<PlayerComponent, BodyComponent>(
    [delta](
        entityx::Entity entity,
        PlayerComponent& player,
        BodyComponent& body) {
      
      // The rotation of the player should be composed of two different
      // rotations: a yaw rotation and a pitch rotation.
      Quaternion pitchRotation(
        std::cos(player.pitch),
        std::sin(player.pitch) * Vector(1.0, 0.0, 0.0));
      Quaternion yawRotation(
        std::cos(player.yaw),
        std::sin(player.yaw) * Vector(0.0, 1.0, 0.0));
      Quaternion totalRotation = yawRotation * pitchRotation;
      body.rotation = totalRotation.unit();
    });
  
  // Now go through every entity that has a player component and an acceleration
  // component and apply the correct linear acceleration to it.
  entities.each<PlayerComponent, BodyComponent, AccelerationComponent>(
    [delta](
        entityx::Entity entity,
        PlayerComponent& player,
        BodyComponent& body,
        AccelerationComponent& acceleration) {
      
      // The acceleration is a vector directed relative to the current rotation
      // of the player.
      Vector acc;
      if (player.forwardKeyPressed) {
        acc -= Vector(0.0, 0.0, 1.0);
      }
      if (player.backwardKeyPressed) {
        acc += Vector(0.0, 0.0, 1.0);
      }
      if (player.rightKeyPressed) {
        acc += Vector(1.0, 0.0, 0.0);
      }
      if (player.leftKeyPressed) {
        acc -= Vector(1.0, 0.0, 0.0);
      }
      if (acc.normSq() != 0.0) {
        acc = player.acceleration * acc.unit();
        // Now the acceleration vector is rotated according to the body's
        // rotation.
        acc = body.rotation.rotate(acc);
      }
      
      acceleration.acceleration = acc;
    });
  
}

void PlayerSystem::receive(KeyboardEvent const& event) {
  
  // Set the keyboard variables of each player component to the appropriate
  // values.
  m_entities->each<PlayerComponent>(
    [event](entityx::Entity entity, PlayerComponent& player) {
      
      bool pressed = (event.action == GLFW_PRESS ||
                      event.action == GLFW_REPEAT);
      if (event.key == player.forwardKey) {
        player.forwardKeyPressed = pressed;
      }
      if (event.key == player.backwardKey) {
        player.backwardKeyPressed = pressed;
      }
      if (event.key == player.leftKey) {
        player.leftKeyPressed = pressed;
      }
      if (event.key == player.rightKey) {
        player.rightKeyPressed = pressed;
      }
    });
}

void PlayerSystem::receive(MouseButtonEvent const& event) {
  
}

void PlayerSystem::receive(MousePositionEvent const& event) {
  
  double lastMouseX = m_lastMouseX;
  double lastMouseY = m_lastMouseY;
  
  m_entities->each<PlayerComponent>(
    [event, &lastMouseX, &lastMouseY](
        entityx::Entity entity,
        PlayerComponent& player) {
      
      double deltaX = event.mouseX - lastMouseX;
      double deltaY = event.mouseY - lastMouseY;
      
      // Adjust the yaw and pitch of the player based on how the mouse has
      // shifted.
      player.yaw += player.sensitivity * deltaX;
      player.pitch -= player.sensitivity * deltaY;
      
      // Make sure that the pitch doesn't go out of bounds (-90 degrees to +90
      // degrees).
      if (player.pitch > M_PI) {
        player.pitch = M_PI;
      }
      if (player.pitch < 0.0) {
        player.pitch = 0.0;
      }
      
      // Wrap the yaw around so that the number doesn't become too big.
      while (player.yaw > 2.0 * M_PI) {
        player.yaw -= 2.0 * M_PI;
      }
      while (player.yaw < 0.0) {
        player.yaw += 2.0 * M_PI;
      }
      
      // Store the mouse x and y for computing the delta next step.
      lastMouseX = event.mouseX;
      lastMouseY = event.mouseY;
    });
  
  m_lastMouseX = lastMouseX;
  m_lastMouseY = lastMouseY;
}

