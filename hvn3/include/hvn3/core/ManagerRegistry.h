#pragma once

#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/utility/TypeIndexer.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hvn3 {

	class IManager;

	template<typename ManagerType>
	class ManagerHandle {

	public:
		typedef ManagerType manager_type;

		ManagerHandle(manager_type* manager) :
			_manager(manager) {}

		manager_type* operator->() { return _manager; }
		const manager_type* operator->() const { return _manager; }
		manager_type& operator*() { return *_manager; }
		const manager_type& operator*() const { return *_manager; }

	private:
		manager_type* _manager;

	};

	class ManagerRegistry {

		struct manager_indexer_family;
		typedef TypeIndexer<manager_indexer_family> manager_indexer;
		template<typename T>
		using handle_type = ManagerHandle<T>;

	public:
		template<typename ManagerType, typename ...Args>
		void Register(Args&&... args) {

			// Make sure the manager inherits from the required base class.
			static_assert(std::is_base_of<IManager, ManagerType>::value, "Type must implement IManager");

			// Make sure that we haven't already registered a manager of this type.
			// Maybe it should be possible to register multiple instances of the same manager in the future?
			assert(!IsRegistered<ManagerType>());

			// Create a manager of the given template type.
			std::unique_ptr<IManager> ptr = std::make_unique<ManagerType>(std::forward<Args>(args)...);

			// If this manager implements an interface, we should register it under that interface.
			_registerInterface<ManagerType>(ptr);

			// Register the manager.
			_registerManager<ManagerType>(std::move(ptr));

		}
		template<typename ManagerType>
		bool Deregister() {

			return _deregisterManager<ManagerType>();

		}

		template<typename ManagerType>
		handle_type<ManagerType> GetManager() {

			// #todo Instead of returning a raw pointer, this should return a manager handle.

			return _findManager<ManagerType>();

		}
		template<typename ManagerType>
		bool IsRegistered() {

			return _findManager<ManagerType>() != nullptr;

		}

		size_t Count() const {
			return _registry.size();
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<IManager>> _registry;
		std::unordered_map<std::type_index, std::vector<void*>> _inteface_registry;

		template<typename ManagerType>
		void _registerManager(std::unique_ptr<IManager>& manager) {

			_registry[typeid(ManagerType)] = std::move(manager);

		}
		template<typename ManagerType>
		void _registerInterface(std::unique_ptr<IManager>& manager) {

			// Get the interface type that this manager implements.

			using interface = typename ManagerType::interface;

			static_assert(std::is_base_of<interface, ManagerType>::value, "Manager type must implement its specified interface");

			if (!std::is_same<interface, void>::value) {

				// If the interface type is valid, get a pointer to this type in the manager object, and add it to the interface registry.

				ManagerType* derived_ptr = static_cast<ManagerType*>(manager.get());
				interface* interface_ptr = static_cast<interface*>(derived_ptr);

				auto it = _inteface_registry.find(typeid(interface));

				if (it == _inteface_registry.end())
					it = _inteface_registry.insert({ typeid(interface), std::vector<void*>() }).first;

				it->second.push_back(interface_ptr);

			}

		}

		template<typename ManagerType> typename std::enable_if<std::is_base_of<IManager, ManagerType>::value, ManagerType*>::type
			_findManager() {

			// Attempt to find the manager directly.

			auto manager_iter = _registry.find(typeid(ManagerType));

			if (manager_iter != _registry.end())
				return static_cast<ManagerType*>((*manager_iter).second.get());

			// Attempt to find a manager implementing the given interface.
			return _findInterface<ManagerType>();

		}
		template<typename ManagerType> typename std::enable_if<!std::is_base_of<IManager, ManagerType>::value, ManagerType*>::type
			_findManager() {

			// If the user has given us a type that doesn't inherit from IManager, they're probably trying to look up a manager by the interface it implements.
			// Since the normal registry stores IManager pointers, which we can't cast to the interface type safely, this is our only option anyway.

			return _findInterface<ManagerType>();

		}
		template<typename InterfaceType>
		InterfaceType* _findInterface() {

			// Attempt to find a manager implementing the given interface.

			auto interface_iter = _inteface_registry.find(typeid(InterfaceType));

			if (interface_iter != _inteface_registry.end() && interface_iter->second.size() > 0)
				return static_cast<InterfaceType*>(interface_iter->second.front());

			return nullptr;

		}

		template<typename ManagerType>
		bool _deregisterManager() {

			// For the time being, managers can not be deregistered by interface alone.
			static_assert(std::is_base_of<IManager, ManagerType>::value, "Type must implement IManager");

			// Find the manager in the registry.

			auto it = _registry.find(typeid(ManagerType));

			if (it == _registry.end())
				return false;

			// Deregister the interface (if it has been registered).
			_deregisterInterface<ManagerType>(static_cast<ManagerType*>(it->second.get()));

			// Deregister the manager.
			_registry.erase(it);

			return true;

		}
		template<typename ManagerType>
		bool _deregisterInterface(ManagerType* managerPtr) {

			using interface = typename ManagerType::interface;

			static_assert(std::is_base_of<interface, ManagerType>::value, "Manager type must implement its specified interface");

			// If this manager type doesn't implement an interface, there is nothing to do.

			if (std::is_same<interface, void>::value)
				return false;

			// Find the collection where the interface pointer is stored in the interface registry.

			auto it = _inteface_registry.find(typeid(interface));

			if (it == _inteface_registry.end())
				return false;

			// Remove the interface pointer from the registry (if it exists).

			interface* interface_ptr = static_cast<interface*>(managerPtr);

			it->second.erase(std::remove(it->second.begin(), it->second.end(), interface_ptr), it->second.end());

			// Remove the collection altogether if it is now empty.
			if (it->second.size() <= 0)
				_inteface_registry.erase(it);

			return true;

		}

	};

	template<typename T>
	std::ostream& operator<<(std::ostream& lhs, const ManagerHandle<T>& rhs) {

		lhs << &*rhs;

		return lhs;

	}

}