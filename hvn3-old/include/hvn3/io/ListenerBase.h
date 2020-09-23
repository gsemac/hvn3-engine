#pragma once
#include "hvn3/core/CoreDefs.h"

#include <functional>
#include <unordered_set>

// #todo This has gotten out of hand.
// There's a better way to handle listeners, and this needs to be cleaned up at some point.

namespace hvn3 {

	template<typename InterfaceType>
	struct ListenerCollectionItem {

		ListenerCollectionItem(InterfaceType* listener) {

			this->listener = listener;
			removed = false;

		}

		bool operator==(const ListenerCollectionItem<InterfaceType>& other) const {
			return listener == other.listener;
		}

		InterfaceType* listener;
		// These can remain mutable so long as they don't participate in the ordering of the set.
		mutable bool removed;

	};

	template<typename T>
	class ListenerBase;

	template <typename InterfaceType>
	class ListenerCollection {

		typedef ListenerCollectionItem<InterfaceType> listener_item_type;
		typedef std::unordered_set<listener_item_type> listener_collection_type;

		struct ListenerRegistry {

			listener_collection_type listeners;

			~ListenerRegistry() {

				// Prevent listeners from deregistering themselves once the registry falls out of scope.
				for (auto i = listeners.begin(); i != listeners.end(); ++i)
					_setDeregister(i->listener, false);

			}

		};

	public:
		static void Add(ListenerBase<InterfaceType>* listener) {

			if (_blocked)
				return;

			_setDeregister(listener, true);

			listener_item_type item(listener);

			_registry.listeners.insert(item);

		}
		static void Remove(const ListenerBase<InterfaceType>* listener) {

			typename listener_collection_type::iterator iter = _registry.listeners.find(const_cast<ListenerBase<InterfaceType>*>(listener));

			if (iter == _registry.listeners.end())
				return;

			if (_active_loops == 0)
				_registry.listeners.erase(iter);
			else
				iter->removed = true;

		}
		static void Remove(const InterfaceType* listener) {

			typename listener_collection_type::iterator iter = _registry.listeners.find(const_cast<InterfaceType*>(listener));

			if (iter == _registry.listeners.end())
				return;

			if (_active_loops == 0)
				_registry.listeners.erase(iter);
			else
				iter->removed = true;

		}
		static typename listener_collection_type::size_type Count() {
			return _registry.listeners.size();
		}

		static void SetBlocked(bool value) {
			_blocked = value;
		}

		static void ForEach(const std::function<bool(InterfaceType*)>& func) {

			++_active_loops;

			for (auto i = _registry.listeners.begin(); i != _registry.listeners.end(); ++i) {

				if (i->removed)
					continue;

				if (func(i->listener) == hvn3::BREAK)
					break;

			}

			--_active_loops;

			if (_active_loops == 0)
				_clearRemovedListeners();

		}

	private:
		static ListenerRegistry _registry;
		static unsigned int _active_loops;
		static bool _blocked;

		static void _setDeregister(InterfaceType* ptr, bool value) {
			static_cast<ListenerBase<InterfaceType>*>(ptr)->_deregister_self = value;
		}
		static void _clearRemovedListeners() {

			for (auto i = _registry.listeners.begin(); i != _registry.listeners.end();)
				if (i->removed)
					i = _registry.listeners.erase(i);
				else
					++i;

		}

	};

	template<typename InterfaceType>
	typename ListenerCollection<InterfaceType>::ListenerRegistry ListenerCollection<InterfaceType>::_registry;
	template<typename InterfaceType>
	unsigned int ListenerCollection<InterfaceType>::_active_loops = 0;
	template<typename InterfaceType>
	bool ListenerCollection<InterfaceType>::_blocked = false;

	template <typename InterfaceType>
	class ListenerBase :
		public InterfaceType {

		friend class hvn3::ListenerCollection<InterfaceType>;

	public:
		ListenerBase() :
			ListenerBase(true) {}
		ListenerBase(bool automatic) {

			_deregister_self = false;

			// Add self to the global collection of listeners.
			if (automatic)
				ListenerCollection<InterfaceType>::Add(this);


		}
		~ListenerBase() {

			/*
			Be careful! Because the registry is static, in some scenarios, it's possible for it to be deinitialized before all instances of the listener class.
			If this happens, we need to make sure that they do not attempt to erase themselves from the deinitalized collection.
			The destructor of the registry object sets a flag in all listeners that can be checked for to see if the registry is still valid.

			This is just one approach to solving the problem-- Another previous approach was to make sure that all listener instances are deinitialized before
			reaching the end of the main function.
			*/

			if (_deregister_self)
				ListenerCollection<InterfaceType>::Remove(this);

		}

	private:
		bool _deregister_self;

	};

}

namespace std {

	template <typename T>
	struct hash<hvn3::ListenerCollectionItem<T>> {

		size_t operator()(const hvn3::ListenerCollectionItem<T>& value) const {
			return std::hash<decltype(value.listener)>{}(value.listener);
		}

	};

}