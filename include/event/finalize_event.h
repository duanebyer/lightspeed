#ifndef __LIGHTSPEED_FINALIZE_EVENT_H_
#define __LIGHTSPEED_FINALIZE_EVENT_H_

#include <entityx/entityx.h>

namespace lightspeed {

struct FinalizeEvent final : public entityx::Event<FinalizeEvent> {
};

}

#endif

