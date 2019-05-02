#pragma once

#include "hvn3/utility/TypeList.h"

#include <type_traits>

namespace hvn3 {

	namespace implementation {

		template<typename EventType>
		class EventListenerBaseMethodHelper {

			typedef typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type parameter_type;

		public:
			virtual void OnEvent(parameter_type ev) {};

		};

		template<typename... FlatEventTypes>
		class EventListenerFlatBase;

		template<typename... FlatEventTypes>
		class EventListenerFlatBase<TypeList<FlatEventTypes...>> :
			public EventListenerBaseMethodHelper<FlatEventTypes>... {

		public:
			using event_types = TypeList<FlatEventTypes...>;

		};

	}

	// Base class for event listeners to be passed into an EventListener object.
	template<typename... EventTypes>
	class EventListenerBase :
		public implementation::EventListenerFlatBase<typename TypeList<EventTypes...>::flatten_type> {
	};

}