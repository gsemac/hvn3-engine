#pragma once
#include <unordered_set>

namespace hvn3 {
	namespace System {

		template <typename T>
		class Listener {

		protected:
			typedef std::unordered_set<T*> listener_collection_type;

		private:
			struct ListenerRegistry {
				listener_collection_type listeners;
				~ListenerRegistry() {

					// Prevent listeners from deregistering themselves once the registry falls out of scope.
					for (auto i = listeners.begin(); i != listeners.end(); ++i)
						static_cast<Listener*>(*i)->_deregister_self = false;

				}
			};

		public:
			Listener() {

				// Add self to the global collection of listeners.
				_registry.listeners.insert(static_cast<T*>(this));
				_deregister_self = true;

			}
			~Listener() {

				/*
				Be careful! Because the registry is static, in some scenarios, it's possible for it to be deinitialized before all instances of the listener class.
				If this happens, we need to make sure that they do not attempt to erase themselves from the deinitalized collection.
				The destructor of the registry object sets a flag in all listeners that can be checked for to see if the registry is still valid.

				This is just one approach to solving the problem-- Another previous approach was to make sure that all listener instances are deinitialized before
				reaching the end of the main function.
				*/

				if (_deregister_self)
					_registry.listeners.erase(static_cast<T*>(this));

			}

		protected:
			static listener_collection_type& GetListeners() {
				return _registry.listeners;
			}

		private:
			static ListenerRegistry _registry;
			bool _deregister_self;

		};

		template<typename T>
		typename Listener<T>::ListenerRegistry Listener<T>::_registry;

	}
}