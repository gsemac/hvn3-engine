#pragma once

#include "core/algorithm.h"
#include "core/type_list.h"
#include "events/ievent_bus.h"
#include "events/event_listener_base_base.h"

#include <algorithm>
#include <cassert>
#include <list>

#include <iostream>

namespace hvn3 {

	namespace events {

		template<typename... EventTypes>
		class EventListenerBase :
			public internal::EventListenerBaseBase<typename core::TypeList<EventTypes...>::flatten_type> {

		public:
			virtual ~EventListenerBase() = default;

		};

	}

}