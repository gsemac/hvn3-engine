#pragma once

#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/EventListenerPriority.h"
#include "hvn3/events/IEventListenerContainer.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace hvn3 {

	template<typename EventType>
	class EventListenerContainer :
		public IEventListenerContainer {

	public:
		// The type of event that this container stores listeners for
		typedef EventType event_type;
		// The type of the actual event handler
		typedef implementation::EventListenerBaseMethodBase<event_type> handler_type;

	private:

		// The type stored inside of the underlying container

		struct Value {

			Value() :
				handler(nullptr),
				enabled(true) {}

			handler_type* handler;
			EventListenerPriority priority;
			bool enabled;

		};

		typedef Value value_type;

		// The type of the underlying container
		typedef std::vector<value_type> container_type;

	public:
		EventListenerContainer() :
			_sort_required(false),
			_remove_required(false),
			_currently_dispatching(false) {
		}

		/*

		class Iterator {

			// The type of the underlying iterator
			typedef typename container_type::iterator underlying_type;

		public:
			typedef listener_type value_type;
			typedef typename underlying_type::iterator_category iterator_category;

			Iterator(underlying_type iterator) :
				_it(iterator) {
			}

			bool operator==(const Iterator& rhs) const {
				return _it == rhs._it;
			}
			bool operator!=(const Iterator& rhs) const {
				return !(*this == rhs);
			}

			typename underlying_type::difference_type operator-(const Iterator& rhs) const {
				return _it - rhs._it;
			}

			Iterator& operator++() {

				++_it;

				return *this;

			}
			Iterator operator++(int) {

				Iterator copy(_it);

				++*this;

				return copy;

			}

			value_type* operator->() {
				return &_it->second;
			}
			const value_type* operator->() const {
				return &_it->second;
			}
			value_type& operator*() {
				return _it->second;
			}
			const value_type& operator*() const {
				return _it->second;
			}

		private:
			underlying_type _it;

		};

		typedef Iterator iterator;
		typedef const Iterator const_iterator;

		*/

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
		void AddListener(handler_type* listener, EventListenerPriority priority) {

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


		}
		bool RemoveListener(handler_type* listener) {

			// Find and disable the listener.
			// Disabled listeners will be removed in one pass later, which allows this method to be called inside of event handlers.

			// It's possible that a listener located at the same memory address is added more than once before we've done a pass to remove disabled listeners.
			// For this reason, remove only the listener that matches the address and is currently enabled.
			// Otherwise, we would keep disabling the same listener, which can cause problems if the intended listener gets allocated.

			auto it = std::find_if(_listeners.begin(), _listeners.end(), [=](const value_type& i) {
				return i.handler == listener && i.enabled;
			});

			assert(it != _listeners.end());

			if (it == _listeners.end())
				return false;

			it->enabled = false;

			_remove_required = true;

			return true;

		}

		/*

		iterator begin() {
			return iterator(_listeners.begin());
		}
		iterator end() {
			return iterator(_listeners.end());
		}
		const_iterator begin() const {
			return iterator(_listeners.begin());
		}
		const_iterator end() const {
			return iterator(_listeners.end());
		}

		*/

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