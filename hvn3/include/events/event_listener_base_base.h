#pragma once

#include "core/type_list.h"

#include <functional>
#include <type_traits>
#include <vector>

namespace hvn3 {

	namespace events {

		class IEventBus;

		template<typename T>
		class EventBus;

	}

	namespace internal {

		template<typename EventType>
		using event_handler_callback_parameter_t = typename std::conditional<std::is_fundamental<EventType>::value, EventType, EventType&>::type;
		template<typename EventType>
		using event_handler_callback_t = std::function<void(event_handler_callback_parameter_t<EventType>)>;

		// This base class is used for each event handler method that should be present in the event listener.

		template<typename EventType>
		class EventListenerBaseMethodBase {

			friend class events::EventBus<EventType>;

		protected:
			typedef event_handler_callback_parameter_t<EventType> parameter_type;

		public:
			virtual ~EventListenerBaseMethodBase() {

				for (events::IEventBus* eventDispatcher : subscriptions)
					eventDispatcher->Unsubscribe(this);

				subscriptions.clear();

			}

			virtual void OnEvent(parameter_type ev) {};

		private:
			std::vector<events::IEventBus*> subscriptions;

			void RegisterSubscription(events::IEventBus* eventDispatcher) {

				subscriptions.push_back(eventDispatcher);

			}
			void UnregisterSubscription(events::IEventBus* eventDispatcher) {

				subscriptions.erase(std::remove(subscriptions.begin(), subscriptions.end(), eventDispatcher), subscriptions.end());

			}

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

}