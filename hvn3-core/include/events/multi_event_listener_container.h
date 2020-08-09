#pragma once

#include "core/type_list.h"
#include "events/event_listener_container.h"

#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace hvn3::events {

	class IUserEvent;

	class MultiEventListenerContainer :
		IEventListenerContainer {

		// The type of the container used for storing and looking up event listener containers.
		typedef std::unordered_map<std::type_index, std::unique_ptr<IEventListenerContainer>> registry_container_type;

	public:
		template<typename ...EventTypes, typename EventListenerType>
		void Subscribe(EventListenerType* listener, EventListenerPriority priority = EventListenerPriority::Normal) {

			SubscribeAll(typename core::TypeList<EventTypes...>::flatten_type(), listener, priority);

		}
		template<typename EventListenerType>
		void SubscribeAll(EventListenerType* listener, EventListenerPriority priority = EventListenerPriority::Normal) {

			SubscribeAll(typename EventListenerType::event_types(), listener, priority);

		}
		template<typename... EventTypes, typename EventListenerType>
		void SubscribeAll(core::TypeList<EventTypes...>, EventListenerType* listener, EventListenerPriority priority) {

			// For each event type accepted by the listener, add the listener to the appropriate container.

			int dummy[] = { 0, (_subscribe<EventTypes>(listener, priority), 0)... };

		}

		template<typename ...EventTypes, typename EventListenerType>
		void Unsubscribe(EventListenerType* listener) {

			UnsubscribeAll(typename core::TypeList<EventTypes...>::flatten_type(), listener);

		}
		template<typename EventListenerType>
		void UnsubscribeAll(EventListenerType* listener) {

			UnsubscribeAll(typename EventListenerType::event_types(), listener);

		}
		template<typename... EventTypes, typename EventListenerType>
		void UnsubscribeAll(core::TypeList<EventTypes...>, EventListenerType* listener) {

			// For each event type accepted by the listener, remove the listener to the appropriate container.

			int dummy[] = { 0, (_unsubscribe<EventTypes>(listener), 0)... };

		}

		template<typename EventType> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type Dispatch(const EventType& ev) {

			// We need to create a copy of the event data so that event handlers can modify it.

			EventType event_data(ev);

			Dispatch((EventType&)ev);

		}
		template<typename EventType> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type Dispatch(EventType& ev) {

			// Since we know the event type, we can dispatch it directly to the correct listener collection.

			using container_type = EventListenerContainer<EventType>;

			container_type* container = _getContainerFromEventType<EventType>(false);

			if (container != nullptr)
				container->Dispatch(ev);

		}
		template<typename EventType, typename ...Args> typename std::enable_if<!std::is_base_of<IUserEvent, EventType>::value, void>::type Dispatch(Args&&... args) {

			EventType event_data(std::forward<Args>(args)...);

			Dispatch(event_data);

		}

		void Dispatch(IUserEvent& ev);

		size_type Count() const override;

	private:
		registry_container_type _registry;

		template<typename EventType, typename EventListenerType>
		void _subscribe(EventListenerType* listener, EventListenerPriority priority) {

			using event_types = typename EventListenerType::event_types;
			using event_type = EventType;
			using event_types_contains = typename event_types::template contains<event_type>;
			using container_type = EventListenerContainer<event_type>;

			static_assert(event_types_contains::value, "The listener does not handle the given event type.");

			// Get or create a container for listeners of this event type.

			container_type* container = _getContainerFromEventType<event_type>(true);

			assert(container != nullptr);

			// Add the listener to the container.

			container->Subscribe(listener, priority);

		}
		template<typename EventType, typename EventListenerType>
		void _unsubscribe(EventListenerType* listener) {

			using event_types = typename EventListenerType::event_types;
			using event_type = EventType;
			using event_types_contains = typename event_types::template contains<event_type>;
			using container_type = EventListenerContainer<event_type>;

			static_assert(event_types_contains::value, "The listener does not handle the given event type.");

			// Get the container for listeners of this event type.

			container_type* container = _getContainerFromEventType<event_type>(false);

			// If no such container exists, we have nothing to do.

			if (container == nullptr)
				return;

			// Remove the listener from the container.

			container->Unsubscribe(listener);

		}

		template<typename EventType>
		EventListenerContainer<EventType>* _getContainerFromEventType(bool createIfNotExists) {

			using event_type = EventType;
			using container_interface_type = IEventListenerContainer;
			using container_type = EventListenerContainer<event_type>;

			// Attempt to find an appropriate container for this listener.
			auto it = _registry.find(typeid(event_type));

			if (it == _registry.end() && createIfNotExists) {

				// If no such container exists, add one.

				std::unique_ptr<container_interface_type> item = std::make_unique<container_type>();

				it = _registry.insert({ typeid(event_type), std::move(item) }).first;

			}

			// Return the container.

			container_type* container = it == _registry.end() ? nullptr : static_cast<container_type*>(it->second.get());

			return container;

		}

	};

}