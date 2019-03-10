#pragma once

#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/utility/TypeIndexer.h"

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hvn3 {

	class ManagerRegistry {

		struct manager_indexer_family;
		typedef TypeIndexer<manager_indexer_family> manager_indexer;

	public:
		template<typename ManagerType, typename ...Args>
		void Register(Args&&... args) {

			// Make sure the manager inherits from the required base class.
			static_assert(std::is_base_of<IManager, ManagerType>::value, "Type must implement IManager");

			// Make sure that we haven't already registered a manager of this type.
			assert(!IsRegistered<ManagerType>());

			// Create a manager of the requested type.

			std::unique_ptr<IManager> ptr = std::make_unique<ManagerType>(std::forward<Args>(args)...);

			// If this manager implements an interface, it should also be added to the interface queues.
			// This allows it to be looked up through its interface.
			_registerManagerToInterfaceRegistry<ManagerType>(ptr.get());

			// Add the manager to registry.
			_registry[typeid(ManagerType)] = std::move(ptr);

		}
		template<typename ManagerType>
		bool Deregister() {

			auto iter = _registry.find(typeid(ManagerType));

			if (iter != _registry.end()) {

				_deregisterManagerFromInterfaceRegistry(iter->second.get());

				_registry.erase(iter);

				return true;

			}

			return false;

		}

		template<typename ManagerType>
		ManagerType* GetManager() {

			// #todo Instead of returning a raw pointer, this should return a manager handle.

			return _findManager<ManagerType>();

		}
		template<typename ManagerType>
		bool IsRegistered() const {

			// Attempt to find the manager directly (without having to do any dynamic casting).
			// Even though we can "get" a manager using dynamic casting, don't use that for checking if a manager exists (performance reasons).

			auto iter = _registry.find(typeid(ManagerType));

			if (iter != _registry.end())
				return true;

			return false;

		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<IManager>> _registry;
		std::vector<std::vector<IManager*>> _inteface_registry;

		template<typename ManagerType>
		ManagerType* _findManager() {

			// Attempt to find the manager directly.

			auto iter = _registry.find(typeid(ManagerType));

			if (iter != _registry.end())
				return static_cast<ManagerType*>((*iter).second.get());

			// Check for the manager in the interface queues.

			return static_cast<ManagerType*>(_getManagerFromInterfaceRegistry<ManagerType>());

			//if (iter == _registry.end()) {

			//	// If we cannot find the manager directly, check for any managers that can be casted to the correct type.

			//	for (auto i = _registry.begin(); i != _registry.end(); ++i) {

			//		ManagerType* dynamic_ptr = dynamic_cast<ManagerType*>(i->second.get());

			//		if (dynamic_ptr != nullptr)
			//			return dynamic_ptr;

			//	}

			//}

		}
		template<typename ManagerType>
		void _registerManagerToInterfaceRegistry(IManager* managerPtr) {

			if (std::is_same<typename ManagerTraits<ManagerType>::interface, void>::value)
				return;

			manager_indexer::index_type index = manager_indexer::GetIndex<typename ManagerTraits<ManagerType>::interface>();

			if (_inteface_registry.size() < index + 1)
				_inteface_registry.resize(index + 1);

			_inteface_registry[index].push_back(managerPtr);

		}
		template<typename ManagerType>
		void _deregisterManagerFromInterfaceRegistry(IManager* managerPtr) {

			if (std::is_same<typename ManagerTraits<ManagerType>::interface, void>::value)
				return nullptr;

			manager_indexer::index_type index = manager_indexer::GetIndex<typename ManagerTraits<ManagerType>::interface>();

			if (index < 0 || index > _inteface_registry.size() || _inteface_registry[index].size() <= 0)
				return;

			_inteface_registry[index].erase(std::remove_if(_inteface_registry[index].begin(), _inteface_registry[index].end(); [=](IManager* i) {
				i == managerPtr; 
			}), _inteface_registry[index].end());

		}
		template<typename ManagerType>
		IManager* _getManagerFromInterfaceRegistry() {

			if (std::is_same<typename ManagerTraits<ManagerType>::interface, void>::value)
				return nullptr;

			manager_indexer::index_type index = manager_indexer::GetIndex<typename ManagerTraits<ManagerType>::interface>();

			if (index < 0 || index > _inteface_registry.size() || _inteface_registry[index].size() <= 0)
				return nullptr;

			return _inteface_registry[index].front();

		}

	};

}