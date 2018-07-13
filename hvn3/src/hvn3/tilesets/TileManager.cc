#include "hvn3/math/MathUtils.h"
#include "hvn3/tilesets/TileManager.h"

namespace hvn3 {

	TileManager::TileManager(const SizeI& room_size, const SizeI& tile_size) :
		TileMap(static_cast<int>(Math::Ceiling(static_cast<float>(room_size.width) / tile_size.width)),
			static_cast<int>(Math::Ceiling(static_cast<float>(room_size.height) / tile_size.height))) {



	}

	ManagerId TileManager::Id() {
		return TILE_MANAGER;
	}

}