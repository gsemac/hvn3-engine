#pragma once

#include "hvn3/events/EventListener.h"
#include "hvn3/events/IEventListenerContainer.h"
#include "hvn3/utility/TypeList.h"
#include "hvn3/utility/UniqueIntegerGenerator.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace hvn3 {

	template<typename ...EventTypes>
	class EventListenerContainer :
		public IEventListenerContainer {

	public:
		typedef EventListener<EventTypes...> listener_type;
		typedef typename listener_type::listener_type listener_base_type;
		typedef std::vector<std::pair<int, listener_type>> container_type;
		typedef int listener_id;
		typedef typename container_type::size_type size_type;

		class ListenerHandle {

		public:
			typedef EventListenerContainer<EventTypes...> parent_type;
			typedef TypeList<EventTypes...> event_types;

			friend class parent_type;

			ListenerHandle() :
				parent(nullptr) {
			}
			ListenerHandle(parent_type* parent, typename container_type::size_type id, typename container_type::size_type index) :
				parent(parent),
				id(id),
				index(index) {

				assert(parent != nullptr);
				assert(index >= 0);

			}

			listener_type* operator->() {

				assert(parent != nullptr);

				listener_type* ptr = parent->_getPointerFromHandle(this);

				assert(ptr != nullptr);

				return ptr;

			}

		private:
			typename container_type::size_type id;
			typename container_type::size_type index;
			parent_type* parent;

		};

		typedef ListenerHandle handle_type;

		class Iterator {

		public:
			typedef listener_type value_type;

			Iterator(typename container_type::iterator iterator) :
				_it(iterator) {}

			bool operator==(const Iterator& rhs) const {
				return _it == rhs._it;
			}
			bool operator!=(const Iterator& rhs) const {
				return !(*this == rhs);
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
			typename container_type::iterator _it;

		};

		typedef Iterator iterator;
		typedef const Iterator const_iterator;

		bool Dispatch(IUserEvent& ev) override {

			bool successful = false;

			int dummy[] = { 0, (successful = _tryDispatch<EventTypes>(ev) || successful, 0)... };

			return successful;

		}
		handle_type AddListener(listener_type&& listener) {

			listener_id id = _id_generator.Next();

			_listeners.push_back(std::make_pair(id, std::move(listener)));

			return handle_type(this, id, Count() - 1);

		}
		bool RemoveListener(handle_type& handle) {

			size_type size_before = Count();

			_listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [&](typename container_type::value_type& n) {
				return n.first == handle.id;
			}), _listeners.end());

			size_type size_now = Count();

			return size_now < size_before;

		}
		bool RemoveListener(const listener_base_type* listener) {

			size_type size_before = Count();

			_listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [&](typename container_type::value_type& n) {
				return n.second.Object() == listener;
			}), _listeners.end());

			size_type size_now = Count();

			return size_now < size_before;

		}

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

		size_type Count() const override {
			return _listeners.size();
		}

	private:
		container_type _listeners;
		UniqueIntegerGenerator<listener_id> _id_generator;

		template<typename EventType>
		bool _tryDispatch(IUserEvent& ev) {

			if (ev.Id() != IUserEvent::event_indexer::GetIndex<EventType>())
				return false;

			EventType* data = static_cast<EventType*>(ev.Data());

			for (auto i = _listeners.begin(); i != _listeners.end(); ++i)
				i->second.OnEvent(*data);

			return true;

		}
		listener_type* _getPointerFromHandle(handle_type* handle) {

			// If the handle is still valid, don't modify it and just return the listener.

			if (handle->index >= 0 && handle->index < Count()) {

				auto it = _listeners.begin() + handle->index;

				if (it->first == handle->id)
					return &it->second;

			}

			// Check if the item the handle refers to is still in the container, and update the handle accordingly.

			typename container_type::size_type index = 0;

			for (auto i = _listeners.begin(); i != _listeners.begin(); ++i, ++index) {

				if (handle->id == i->first) {

					handle->index = index;

					return &i->second;

				}

			}

			// If the item doesn't exist, invalidate the handle.

			*handle = handle_type();

			return nullptr;

		}

	};

}