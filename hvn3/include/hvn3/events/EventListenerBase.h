#pragma once

#include "hvn3/utility/TypeList.h"

#include <type_traits>

namespace hvn3 {

	namespace implementation {

		// This base class is used for each event handler method that should be present in the event listener.

		template<typename EventType>
		class EventListenerBaseMethodBase {

		protected:
			typedef typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type parameter_type;

		public:
			virtual void OnEvent(parameter_type ev) {};

		};

		template<typename... FlatEventTypes>
		class EventListenerBaseBase;

		template<typename... FlatEventTypes>
		class EventListenerBaseBase<TypeList<FlatEventTypes...>> :
			public EventListenerBaseMethodBase<FlatEventTypes>... {

		public:
			using event_types = TypeList<FlatEventTypes...>;

		};

	}

	template<typename... EventTypes>
	class EventListenerBase :
		public implementation::EventListenerBaseBase<typename TypeList<EventTypes...>::flatten_type> {
	};

}