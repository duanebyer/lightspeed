#ifndef __LIGHTSPEED_RENDER_SYSTEM_H_
#define __LIGHTSPEED_RENDER_SYSTEM_H_

#include <unordered_map>

#include <entityx/entityx.h>

#include "internal/opengl.h"

#include "component/body_component.h"
#include "component/model_component.h"
#include "component/timeline_component.h"

#include "event/finalize_event.h"
#include "event/initialize_event.h"
#include "event/render_event.h"

namespace lightspeed {

class RenderSystem final : public entityx::System<RenderSystem>,
                           public entityx::Receiver<RenderSystem> {
  
public:
  
  void configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) override;
  
  void update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) override;
  
  void receive(InitializeEvent const& event);
  void receive(FinalizeEvent const& event);
  void receive(RenderEvent const& event);
  
  void receive(entityx::ComponentAddedEvent<ModelComponent> const& event);
  void receive(entityx::ComponentRemovedEvent<ModelComponent> const& event);
  
  void receive(
    entityx::ComponentAddedEvent<
      TimelineComponent<BodyComponent> > const& event);
  void receive(
    entityx::ComponentRemovedEvent<
      TimelineComponent<BodyComponent> > const& event);
  
private:
  
  entityx::EntityManager* m_entities;
  entityx::EventManager* m_events;
  
  std::unordered_map<ModelComponent const*, GLuint> m_vertexBuffers;
  std::unordered_map<
    TimelineComponent<BodyComponent> const*, GLuint> m_timelineBuffers;
  GLuint m_renderRelativisticShader;
  
};

}

#endif

