#ifndef __LIGHTSPEED_TIMELINE_COMPONENT_H_
#define __LIGHTSPEED_TIMELINE_COMPONENT_H_

#include <deque>
#include <utility>

namespace lightspeed {

/**
 * \brief Stores a history of the past values of a component of an entity over
 * time.
 * 
 * The history takes the form of a map from times to the value of the component
 * at that time. Only values up to a certain age are stored.
 */
template<typename T>
struct TimelineComponent final {
  
  TimelineComponent(double timeInterval) :
      timeline(),
      timeInterval(timeInterval) {
  }
  
  std::deque<std::pair<double, T> > timeline;
  double timeInterval;
  
};

}

#endif

