#pragma once

#include "events/ievent_bus.h"
#include "events/event_listener_base_base.h"
#include "events/event_listener_priority.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>
#include <vector>

namespace hvn3::events {

	template<typename EventType>
	class EventBus :
		public IEventBus {

	protected:
		typedef EventType event_type; // The type of event that this container stores listeners for
		typedef internal::EventListenerBaseMethodBase<event_type> handler_type; // The type of the actual event handler
		typedef typename std::conditional<std::is_fundamental_v<event_type>, event_type, event_type&>::type parameter_type;
		typedef std::function<void(parameter_type)> callback_type;

	private:

		// The type stored inside of the underlying container

		struct Value {

			Value() :
				handler(nullptr),
				enabled(true),
				priority(EventListenerPriority::Normal) {
			}

			handler_type* handler;
			callback_type callback;
			EventListenerPriority priority;
			bool enabled;

		};

		typedef Value value_type;
		typedef std::vector<value_type> container_type; // The type of the underlying container

	public:
		EventBus() :
			sortRequired(false),
			removeRequired(false),
			currentlyDispatching(false) {
		}

		~EventBus() {

			for (const value_type& i : listeners)
				if (i.enabled)
					i.handler->UnregisterSubscription(this);

		}

		void Dispatch(parameter_type ev) {

			// If the listener collection needs to be sorted, do that before dispatching.
			if (sortRequired)
				SortListenersByPriority();

			// If there are listeners that need to be removed, do that before dispatching.
			if (removeRequired)
				RemoveDisabledListeners();

			sortRequired = false;
			removeRequired = false;

			// Notify all listeners.
			// Listeners might be disabled by other listeners, so we still need to check for that.

			currentlyDispatching = true;

			for (auto& listener : listeners) {

				if (!listener.enabled)
					continue;

				if (listener.handler != nullptr)
					listener.handler->OnEvent(ev);
				else if (listener.callback)
					listener.callback(ev);

			}

			currentlyDispatching = false;

		}
		void Subscribe(handler_type* listener, EventListenerPriority priority = EventListenerPriority::Normal) {

			// We would run into issues with iterators being invalidated if this were to be called inside of the event handler.
			// This could be solved in a number of ways, but generally one would not attempt to subscribe to an event that's currently being handled.
			// Therefore, we will simply forbid adding new listeners inside of event handlers.

			assert(!currentlyDispatching);

			// Listeners should only be subscribed once.

			assert(std::none_of(listeners.begin(), listeners.end(), [=](const value_type& x) {
				return x.handler == listener && x.enabled;
				}));

			value_type item;

			item.handler = listener;
			item.priority = priority;

			if (listeners.size() > 0 && listeners.back().priority < priority)
				sortRequired = true;

			listeners.push_back(item);

			listener->RegisterSubscription(this);

		}
		void Subscribe(const callback_type& callback, EventListenerPriority priority = EventListenerPriority::Normal) {

			assert(!currentlyDispatching);

			value_type item;

			item.callback = callback;
			item.priority = priority;

			listeners.push_back(item);

		}
		bool Unsubscribe(handler_type* listener) {

			// Find and disable the listener.
			// Disabled listeners will be removed in one pass later, which allows this method to be called inside of event handlers.

			// It's possible that a listener located at the same memory address is added more than once before we've done a pass to remove disabled listeners.
			// For this reason, remove only the listener that matches the address and is currently enabled.
			// Otherwise, we would keep disabling the same listener, which can cause problems if the intended listener gets allocated.

			auto it = std::find_if(listeners.begin(), listeners.end(), [=](const value_type& i) {
				return i.handler == listener && i.enabled;
				});

			if (it == listeners.end())
				return false;

			it->enabled = false;

			removeRequired = true;

			listener->UnregisterSubscription(this);

			return true;

		}
		bool Unsubscribe(void* eventListener) override {

			handler_type* handler = static_cast<handler_type*>(eventListener);

			return Unsubscribe(handler);

		}

		size_type Count() const override {

			return listeners.size();

		}

	private:
		container_type listeners;
		bool sortRequired;
		bool removeRequired;
		bool currentlyDispatching;

		void SortListenersByPriority() {

			std::sort(listeners.begin(), listeners.end(), [](const value_type& lhs, const value_type& rhs) {
				return lhs.priority > rhs.priority;
				});

		}
		void RemoveDisabledListeners() {

			listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [](const value_type& i) {
				return !i.enabled;
				}), listeners.end());

		}

	};

}