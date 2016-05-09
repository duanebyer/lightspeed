#ifndef __LIGHTSPEED_TIMELINE_COMPONENT_H_
#define __LIGHTSPEED_TIMELINE_COMPONENT_H_

#include <utility>

#include <entityx/entityx.h>

#include "component/timeline_component.h"

namespace lightspeed {

template<typename T>
class TimelineSystem final : public entityx::System<TimelineSystem<T> > {
  
public:
  
  TimelineSystem() :
      m_currentTime(0.0) {
  }
  
  void update(
      entityx::EntityManager& entities,
      entityx::EventManager& events,
      entityx::TimeDelta delta) override {
    
    // For each entity with a timeline component, add the current value into the
    // timeline so that it can be retrieved later. If there are any values in
    // the timeline that are older than should be stored, remove them.
    entities.each<TimelineComponent<T>, T>(
      [delta, m_currentTime](
          entityx::Entity entity,
          TimelineComponent<T>& timeline,
          T value) {
        
        // Shift the old entries backwards in time, and cull any of the entries
        // that are older than a certain amount.
        unsigned int i = timeline.timeline.size() - 1;
        do {
          timeline.timeline[i].first -= delta;
          if (timeline.timeline[i].first < -timeline.timeInterval) {
            timeline.pop_front();
          }
        } while ((i--) != 0);
        
        // Add the new entry to the timeline.
        timeline.timeline.push_back(make_pair(0.0, value));
    });
  }
  
};

}

#endif

