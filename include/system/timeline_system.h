#ifndef __LIGHTSPEED_TIMELINE_SYSTEM_H_
#define __LIGHTSPEED_TIMELINE_SYSTEM_H_

#include <utility>

#include <entityx/entityx.h>

#include "component/timeline_component.h"

#include "event/relativistic_update_event.h"

namespace lightspeed {

template<typename T>
class TimelineSystem final : public entityx::System<TimelineSystem<T> >,
                             public entityx::Receiver<TimelineSystem<T> > {
  
public:
  
  void configure(
      entityx::EntityManager& entities,
      entityx::EventManager& events) override {
    
    m_entities = &entities;
    m_events = &events;
    
    events.subscribe<RelativisticUpdateEvent>(*this);
  }
  
  void receive(RelativisticUpdateEvent const& event) {
    
    // For each entity with a timeline component, add the current value into the
    // timeline so that it can be retrieved later. If there are any values in
    // the timeline that are older than should be stored, remove them.
    m_entities->each<TimelineComponent<T>, T>(
      [event](entityx::Entity entity, TimelineComponent<T>& timeline, T value) {
        
        // Add the new entry to the timeline.
        timeline.timeline.push_back(std::make_pair(event.deltaPrime, value));
        
        // Shift the old entries backwards in time, and cull any of the entries
        // that are older than a certain amount.
        unsigned int i = timeline.timeline.size() - 1;
        do {
          timeline.timeline[i].first -= event.deltaPrime;
          if (timeline.timeline[i].first < -timeline.timeInterval) {
            timeline.timeline.pop_front();
          }
        } while ((i--) != 0);
    });
  }
  
  void update(
      entityx::EntityManager& entities,
      entityx::EventManager& events,
      entityx::TimeDelta delta) override {
  }
  
private:
  
  entityx::EntityManager* m_entities;
  entityx::EventManager* m_events;
  
};

}

#endif

