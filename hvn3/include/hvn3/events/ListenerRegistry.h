#pragma once

#include "hvn3/events/ListenerContainer.h"
#include "hvn3/events/UserEvent.h"

#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

namespace hvn3 {

	class ListenerRegistry {

	public:
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
		typename ListenerContainer<EventTypes...>::handle_type AddListener(Args... args) {

			using listener_type = EventListener<EventTypes...>;
			using registry_type = ListenerContainer<EventTypes...>;
			using handle_type = typename ListenerContainer<EventTypes...>::handle_type;

			listener_type listener(args...);

			auto it = _registry.find(typeid(listener_type));

			if (it == _registry.end()) {

				std::unique_ptr<IListenerContainer> item = std::make_unique<registry_type>();

				it = _registry.insert({ typeid(listener_type), std::move(item) }).first;

			}

			registry_type* listeners = static_cast<registry_type*>(it->second.get());

			return listeners->AddListener(std::move(listener));

		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<IListenerContainer>> _registry;

	};

}