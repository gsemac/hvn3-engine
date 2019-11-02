#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/tiles/TileMap.h"
#include "hvn3/tiles/TileMapRenderer.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class TileManager :
		public IManager,
		public TileMap,
		public TileMapRenderer,
		public IDrawable {

	public:
		TileManager(const SizeI& room_size, const SizeI& tile_size);

		void OnDraw(DrawEventArgs& e) override;

		void SetTileSize(const SizeI& tile_size);

	private:
		SizeI _room_size;

	};

	template<>
	struct ManagerIdTraits<TILE_MANAGER> {
		typedef TileManager type;
	};

}