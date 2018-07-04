#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/tilesets/TileMapRenderer.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class TileManager :
		public ManagerBase,
		public TileMap,
		public TileMapRenderer {

	public:
		TileManager(const SizeI& room_size, const SizeI& tile_size);
		
		static ManagerId Id();

	private:

	};

}