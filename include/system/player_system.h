#ifndef __LIGHTSPEED_PLAYER_SYSTEM_H_
#define __LIGHTSPEED_PLAYER_SYSTEM_H_

#include <entityx/entityx.h>

#include "event/keyboard_event.h"
#include "event/mouse_button_event.h"
#include "event/mouse_position_event.h"

namespace lightspeed {

class PlayerSystem final : public entityx::System<PlayerSystem>,
                           public entityx::Receiver<PlayerSystem> {
  
public:
  
  PlayerSystem() :
      m_lastMouseX(0.0),
      m_lastMouseY(0.0) {
  }
  
  void configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) override;
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
  void receive(KeyboardEvent const& event);
  void receive(MouseButtonEvent const& event);
  void receive(MousePositionEvent const& event);
  
private:
  
  entityx::EntityManager* m_entities;
  entityx::EventManager* m_events;
  
  double m_lastMouseX;
  double m_lastMouseY;
  
};

}

#endif

