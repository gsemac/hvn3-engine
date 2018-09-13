#pragma once
#include "hvn3/core/IManager.h"

#include <memory>

namespace hvn3 {

	typedef unsigned int ManagerId;

	enum BUILTIN_MANAGER_ID : ManagerId {

		GAME_MANAGER,
		ROOM_MANAGER,
		OBJECT_MANAGER,
		COLLISION_MANAGER,
		PHYSICS_MANAGER,
		TILE_MANAGER,

		BASIC_LIGHTING_MANAGER,

		MINIMUM_USER_MANAGER_ID

	};

	class IGameManager;

	template<ManagerId MANAGER_ID>
	struct ManagerTraits {
		typedef void type;
	};

}