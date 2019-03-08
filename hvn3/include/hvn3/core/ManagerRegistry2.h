#pragma once

#include "hvn3/core/IManager.h"

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace hvn3 {

	class ManagerRegistry2 {

	public:
		template<typename ManagerType, typename ...Args>
		void Register(Args&&... args) {

			// Make sure the manager inherits from the required base class.
			static_assert(std::is_base_of<IManager, ManagerType>::value, "Manager must inherit from IManager");

			// Make sure that we haven't already registered a manager of this type.
			assert(!IsRegistered<ManagerType>());

			// Create a manager of the requested type.

			std::unique_ptr<IManager> ptr = std::make_unique<ManagerType>(std::forward<Args>(args)...);

			_registry[typeid(ManagerType)] = std::move(ptr);

		}
		template<typename ManagerType>
		bool Deregister() {

			auto iter = _registry.find(typeid(ManagerType));

			if (iter != _registry.end()) {

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

		template<typename ManagerType>
		ManagerType* _findManager() {

			// Attempt to find the manager directly (without having to do any dynamic casting).

			auto iter = _registry.find(typeid(ManagerType));

			if (iter != _registry.end())
				return static_cast<ManagerType*>((*iter).second.get());

			return nullptr;

			//if (iter == _registry.end()) {

			//	// If we cannot find the manager directly, check for any managers that can be casted to the correct type.

			//	for (auto i = _registry.begin(); i != _registry.end(); ++i) {

			//		ManagerType* dynamic_ptr = dynamic_cast<ManagerType*>(i->second.get());

			//		if (dynamic_ptr != nullptr)
			//			return dynamic_ptr;

			//	}

			//}

		}

	};

}