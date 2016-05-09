#ifndef __LIGHTSPEED_INITIALIZE_EVENT_H_
#define __LIGHTSPEED_INITIALIZE_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct InitializeEvent final : public entityx::Event<InitializeEvent> {
};

}

#endif

