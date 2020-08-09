#pragma once

#include "core/type_list.h"

#include <type_traits>

namespace hvn3::events {

	namespace impl {

		// This base class is used for each event handler method that should be present in the event listener.

		template<typename EventType>
		class EventListenerBaseMethodBase {

		protected:
			typedef typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type parameter_type;

		public:
			virtual void OnEvent(parameter_type ev) {};

		};

		// This base class is used to flatten the list of event types.

		template<typename... FlatEventTypes>
		class EventListenerBaseBase;

		template<typename... FlatEventTypes>
		class EventListenerBaseBase<core::TypeList<FlatEventTypes...>> :
			public EventListenerBaseMethodBase<FlatEventTypes>... {

		public:
			using event_types = core::TypeList<FlatEventTypes...>;

		};

	}

	template<typename... EventTypes>
	class EventListenerBase :
		public impl::EventListenerBaseBase<typename core::TypeList<EventTypes...>::flatten_type> {
	};

}