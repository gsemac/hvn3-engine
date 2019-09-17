#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"

namespace hvn3 {

	class PhysicsManager_ :
		public ManagerBase<>,
		public EventListenerBase<events::UpdateEvents> {

	public:
		PhysicsManager_();

	};

}