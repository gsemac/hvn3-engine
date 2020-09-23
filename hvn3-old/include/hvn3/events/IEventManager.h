#pragma once

#include "hvn3/core/IManager.h"

namespace hvn3 {

	class EventListenerRegistry;

	class IEventManager {

	public:
		virtual EventListenerRegistry& GetListenerRegistry() = 0;

	};

}