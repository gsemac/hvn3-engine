#pragma once

#include "events/ievent_dispatcher.h"
#include "events/event_listener_base_base.h"
#include "events/event_listener_priority.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace hvn3::events {

	template<typename EventType>
	class EventDispatcher :
		public IEventDispatcher {

	public:
		// The type of event that this container stores listeners for
		typedef EventType event_type;
		// The type of the actual event handler
		typedef internal::EventListenerBaseMethodBase<event_type> handler_type;

	private:

		// The type stored inside of the underlying container

		struct Value {

			Value() :
				handler(nullptr),
				enabled(true),
				priority(EventListenerPriority::Normal) {}

			handler_type* handler;
			EventListenerPriority priority;
			bool enabled;

		};

		typedef Value value_type;

		// The type of the underlying container
		typedef std::vector<value_type> container_type;

	public:
		EventDispatcher() :
			_sort_required(false),
			_remove_required(false),
			_currently_dispatching(false) {
		}

		~EventDispatcher() {

			for (const value_type& i : _listeners)
				if (i.enabled)
					i.handler->UnregisterSubscription(this);

		}

		void Dispatch(typename std::conditional<std::is_fundamental<event_type>::value, event_type, event_type&>::type ev) {

			// If the listener collection needs to be sorted, do that before dispatching.
			if (_sort_required)
				_sortListenersByPriority();

			// If there are listeners that need to be removed, do that before dispatching.
			if (_remove_required)
				_removeDisabledListeners();

			_sort_required = false;
			_remove_required = false;

			// Notify all listeners.
			// Listeners might be disabled by other listeners, so we still need to check for that.

			_currently_dispatching = true;

			for (auto i = _listeners.begin(); i != _listeners.end(); ++i)
				if (i->enabled)
					i->handler->OnEvent(ev);

			_currently_dispatching = false;

		}
		void Subscribe(handler_type* listener, EventListenerPriority priority = EventListenerPriority::Normal) {

			// We would run into issues with iterators being invalidated if this were to be called inside of the event handler.
			// This could be solved in a number of ways, but generally one would not attempt to subscribe to an event that's currently being handled.
			// Therefore, we will simply forbid adding new listeners inside of event handlers.

			assert(!_currently_dispatching);

			// Listeners should only be subscribed once.

			assert(std::none_of(_listeners.begin(), _listeners.end(), [=](const value_type& x) {
				return x.handler == listener && x.enabled;
				}));

			value_type item;
			item.handler = listener;
			item.priority = priority;

			if (_listeners.size() > 0 && _listeners.back().priority < priority)
				_sort_required = true;

			_listeners.push_back(item);
	
			listener->RegisterSubscription(this);

		}
		bool Unsubscribe(handler_type* listener) {

			// Find and disable the listener.
			// Disabled listeners will be removed in one pass later, which allows this method to be called inside of event handlers.

			// It's possible that a listener located at the same memory address is added more than once before we've done a pass to remove disabled listeners.
			// For this reason, remove only the listener that matches the address and is currently enabled.
			// Otherwise, we would keep disabling the same listener, which can cause problems if the intended listener gets allocated.

			auto it = std::find_if(_listeners.begin(), _listeners.end(), [=](const value_type& i) {
				return i.handler == listener && i.enabled;
				});

			if (it == _listeners.end())
				return false;

			it->enabled = false;

			_remove_required = true;

			listener->UnregisterSubscription(this);

			return true;

		}
		bool Unsubscribe(void* eventListener) override {

			handler_type* handler = static_cast<handler_type*>(eventListener);
		
			return Unsubscribe(handler);

		}

		size_type Count() const override {

			return _listeners.size();

		}

	private:
		container_type _listeners;
		bool _sort_required;
		bool _remove_required;
		bool _currently_dispatching;

		void _sortListenersByPriority() {

			std::sort(_listeners.begin(), _listeners.end(), [](const value_type& lhs, const value_type& rhs) {
				return lhs.priority > rhs.priority;
				});

		}
		void _removeDisabledListeners() {

			_listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [](const value_type& i) {
				return !i.enabled;
				}), _listeners.end());

		}

	};

}