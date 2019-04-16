#pragma once

#include "hvn3/events/EventListenerContainer.h"
#include "hvn3/events/UserEvent.h"
#include "hvn3/utility/TypeList.h"

#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

#include <iostream>

namespace hvn3 {

	class EventListenerRegistry {

	public:
		template<typename ...EventTypes>
		using listener_type = EventListener<EventTypes...>;
		template<typename ...EventTypes>
		using registry_type = EventListenerContainer<EventTypes...>;
		template<typename ...EventTypes>
		using handle_type = typename registry_type<EventTypes...>::handle_type;

		template<typename EventType> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type
			Dispatch(const EventType& ev) {

			UserEvent<EventType> event(ev);

			Dispatch(event);

		}
		void Dispatch(IUserEvent& ev) {

			for (auto i = _registry.begin(); i != _registry.end(); ++i)
				i->second->Dispatch(ev);

		}

		template<typename ...EventTypes, typename ...Args>
		handle_type<EventTypes...> AddListener(Args... args) {

			using listener_type = listener_type<EventTypes...>;
			using registry_type = registry_type<EventTypes...>;

			listener_type listener(args...);
			registry_type* registry = _getRegistry<EventTypes...>();

			return registry->AddListener(std::move(listener));

		}
		template<typename... EventTypes, typename EventListenerType>
		handle_type<EventTypes...> AddListener(TypeList<EventTypes...>, EventListenerType* listener) {

			return AddListener<EventTypes...>(listener);

		}
		template<typename EventListenerType>
		auto AddListener(EventListenerType* listener) {

			return AddListener(typename EventListenerType::event_types(), listener);

		}

		template<typename... EventTypes>
		bool RemoveListener(handle_type<EventTypes...>& handle) {

			using registry_type = registry_type<EventTypes...>;

			registry_type* registry = _getRegistry<EventTypes...>();

			return registry->RemoveListener(handle);

		}
		template<typename... EventTypes, typename EventListenerHandleType>
		bool RemoveListener(TypeList<EventTypes...>, EventListenerHandleType& handle) {

			return RemoveListener<EventTypes...>(handle);

		}
		template<typename EventListenerHandleType>
		bool RemoveListener(EventListenerHandleType& handle) {

			return RemoveListener(typename EventListenerHandleType::event_types(), handle);

		}
		template<typename... EventTypes, typename EventListenerType>
		bool RemoveListener(TypeList<EventTypes...>, const EventListenerType* listener) {

			using registry_type = registry_type<EventTypes...>;

			registry_type* registry = _getRegistry<EventTypes...>();

			return registry->RemoveListener(listener);

		}
		template<typename EventListenerType>
		bool RemoveListener(const EventListenerType* listener) {

			return RemoveListener(typename EventListenerType::event_types(), listener);

		}
		
	private:
		std::unordered_map<std::type_index, std::unique_ptr<IEventListenerContainer>> _registry;

		template<typename ...EventTypes>
		EventListenerContainer<EventTypes...>* _getRegistry() {

			using listener_type = EventListener<EventTypes...>;
			using registry_type = EventListenerContainer<EventTypes...>;

			auto it = _registry.find(typeid(listener_type));

			if (it == _registry.end()) {

				std::unique_ptr<IEventListenerContainer> item = std::make_unique<registry_type>();

				it = _registry.insert({ typeid(listener_type), std::move(item) }).first;

			}

			registry_type* registry = static_cast<registry_type*>(it->second.get());

			return registry;

		}

	};

}