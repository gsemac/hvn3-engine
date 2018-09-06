#pragma once
#include <unordered_set>

namespace hvn3 {

	template<typename T>
	class ListenerBase;

	template <typename InterfaceType>
	class ListenerCollection {

	public:
		typedef std::unordered_set<InterfaceType*> listener_collection_type;

	private:
		struct ListenerRegistry {

			listener_collection_type listeners;

			~ListenerRegistry() {

				// Prevent listeners from deregistering themselves once the registry falls out of scope.
				for (auto i = listeners.begin(); i != listeners.end(); ++i)
					_setDeregister(*i, false);

			}

		};

	public:
		static void Add(ListenerBase<InterfaceType>* listener) {

			_setDeregister(listener, true);

			_registry.listeners.insert(listener);

		}
		static void Remove(const ListenerBase<InterfaceType>* listener) {
			_registry.listeners.erase(const_cast<ListenerBase<InterfaceType>*>(listener));
		}
		static void Remove(const InterfaceType* listener) {
			_registry.listeners.erase(const_cast<InterfaceType*>(listener));
		}
		static typename listener_collection_type::size_type Count() {
			return _registry.size();
		}

		static const listener_collection_type& Listeners() {
			return _registry.listeners;
		}

	private:
		static ListenerRegistry _registry;

		static void _setDeregister(InterfaceType* ptr, bool value) {
			static_cast<ListenerBase<InterfaceType>*>(ptr)->_deregister_self = value;
		}

	};

	template<typename T>
	typename ListenerCollection<T>::ListenerRegistry ListenerCollection<T>::_registry;

	template <typename InterfaceType>
	class ListenerBase :
		public InterfaceType {

		friend class hvn3::ListenerCollection<InterfaceType>;

	public:
		ListenerBase() {
			// Add self to the global collection of listeners.
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