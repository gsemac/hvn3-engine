#pragma once

#include "hvn3/events/EventListener.h"
#include "hvn3/events/IListenerContainer.h"
#include "hvn3/utility/UniqueIntegerGenerator.h"

#include <cassert>
#include <utility>
#include <vector>

namespace hvn3 {

	template<typename ...EventTypes>
	class ListenerContainer :
		public IListenerContainer {

	public:
		typedef EventListener<EventTypes...> listener_type;
		typedef std::vector<std::pair<int, listener_type>> container_type;
		typedef int listener_id;

		class ListenerHandle {

		public:
			typedef ListenerContainer<EventTypes...> parent_type;

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

		typename container_type::size_type Count() const {
			return _listeners.size();
		}

	private:
		container_type _listeners;
		hvn3::UniqueIntegerGenerator<listener_id> _id_generator;

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