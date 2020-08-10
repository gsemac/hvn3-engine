#pragma once

#include "core/algorithm.h"
#include "core/type_list.h"
#include "events/event_dispatcher.h"
#include "events/event_listener_base_base.h"
#include "events/ievent_listener.h"

#include <algorithm>
#include <cassert>
#include <list>

#include <iostream>

namespace hvn3 {

	namespace events {

		template<typename... EventTypes>
		class EventListenerBase :
			public internal::EventListenerBaseBase<typename core::TypeList<EventTypes...>::flatten_type> {
		};

	}

}