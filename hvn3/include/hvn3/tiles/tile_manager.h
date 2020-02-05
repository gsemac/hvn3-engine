#pragma once

#include "hvn3/services/di_service_container.h"
#include "hvn3/tiles/itile_manager.h"
#include "hvn3/tiles/tile_map_renderer.h"

namespace hvn3::tiles {

	class TileManager :
		public ITileManager {

	public:
		HVN3_INJECT(TileManager());

		TileMap::Layer& AddLayer(int depth) override;
		TileMap::Layer& GetLayer(int depth) override;
		const TileMap::Layer& GetLayer(int depth) const override;
		bool RemoveLayer(int depth) override;

		void Resize(int rows, int columns) override;

		void Clear() override;

	private:
		TileMap tileMap;
		TileMapRenderer renderer;

	};

}