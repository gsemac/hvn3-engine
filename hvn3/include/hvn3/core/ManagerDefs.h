#pragma once
#include <memory>

namespace hvn3 {

	class IManager;

	typedef unsigned int ManagerId;

	typedef std::unique_ptr<IManager> IManagerPtr;
	template <typename ManagerType>
	using ManagerPtr = std::unique_ptr<ManagerType>;

	enum : ManagerId {
		GAME_MANAGER,
		ROOM_MANAGER,
		OBJECT_MANAGER,
		COLLISION_MANAGER,
		PHYSICS_MANAGER,
		TILE_MANAGER,
		BASIC_LIGHTING_MANAGER,
		VIEW_MANAGER,
		BACKGROUND_MANAGER,
		PARTICLE_MANAGER,
		ASSET_MANAGER
	};

	template<ManagerId MANAGER_TYPE>
	struct ManagerIdTraits {
		typedef void type;
	};

	template<typename ManagerType, typename... Args>
	ManagerPtr<ManagerType> make_manager(Args&&... args) {

		ManagerPtr<ManagerType> ptr = std::make_unique<ManagerType>(std::forward<Args>(args)...);

		return ptr;

	}

}