#pragma once
#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace hvn3 {

	class IGameManager;

	class ManagerRegistry {

		typedef IManager manager_interface_type;
		typedef std::unordered_map<std::type_index, std::unique_ptr<manager_interface_type>> registry_type;

	public:
		ManagerRegistry(std::function<void(IManager*)>&& register_callback) {

			_game_manager = nullptr;
			_register_callback = std::move(register_callback);

		}
		ManagerRegistry(IGameManager* game_manager, std::function<void(IManager*)>&& register_callback) {

			_game_manager = game_manager;
			_register_callback = std::move(register_callback);

		}

		template<typename ManagerType>
		void Register(std::unique_ptr<ManagerType>& manager) {

			if (_register_callback)
				_register_callback(manager.get());

			_registry[typeid(ManagerType)] = std::move(manager);

		}

		template<typename InterfaceType, typename ManagerType>
		void Register(std::unique_ptr<ManagerType>& manager) {

			static_assert(std::is_base_of<InterfaceType, ManagerType>::value, "The registered manager must implement the given interface");

			if (_register_callback)
				_register_callback(manager.get());

			_registry[typeid(InterfaceType)] = std::move(manager);

		}

		template<ManagerId MANAGER_ID, typename ManagerType>
		void Register(std::unique_ptr<ManagerType>& manager) {

			using interface_type = typename ManagerIdTraits<MANAGER_ID>::type;

			Register<interface_type, ManagerType>(std::move(manager));

		}

		template <typename InterfaceType>
		InterfaceType& Get() const {

			auto iter = _registry.find(typeid(InterfaceType));

			HVN3_ASSERT(iter != _registry.end(), "The requested manager does not exist in the registry.");

			return *static_cast<InterfaceType*>(iter->second.get());

		}
		template<>
		IGameManager& Get() const {

			HVN3_ASSERT(_game_manager != nullptr, "Game manager was nullptr.");

			return *_game_manager;

		}

		template <ManagerId MANAGER_ID>
		typename ManagerIdTraits<MANAGER_ID>::type& Get() const {

			using interface_type = typename ManagerIdTraits<MANAGER_ID>::type;

			return Get<interface_type>();

		}

		template <typename InterfaceType>
		bool IsRegistered() const {
			return _registry.count(typeid(InterfaceType)) > 0;
		}
		template <>
		bool IsRegistered<IGameManager>() const {
			return _game_manager != nullptr;
		}

		template <ManagerId MANAGER_ID>
		bool IsRegistered() const {

			using interface_type = typename ManagerIdTraits<MANAGER_ID>::type;

			return IsRegistered<interface_type>();

		}

		registry_type::iterator begin() {
			return _registry.begin();
		}
		registry_type::iterator end() {
			return _registry.end();
		}
		registry_type::const_iterator begin() const {
			return _registry.begin();
		}
		registry_type::const_iterator end() const {
			return _registry.end();
		}

	private:
		registry_type _registry;
		IGameManager* _game_manager;
		std::function<void(IManager*)> _register_callback;

	};

}