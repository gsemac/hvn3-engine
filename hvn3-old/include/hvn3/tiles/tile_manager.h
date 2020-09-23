#pragma once

#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/services/di_service_container.h"
#include "hvn3/tiles/itile_manager.h"
#include "hvn3/tiles/tile_map_renderer.h"

namespace hvn3::tiles {

	class TileManager :
		public ITileManager,
		public EventListenerBase<events::DrawEvents> {

	public:
		HVN3_INJECT(TileManager());

		void OnEvent(DrawEventArgs& e) override;

		void AddTileset(const Tileset& tileset) override;

		TileMap::Layer& AddLayer(int depth) override;
		TileMap::Layer& AddLayer(int rows, int columns, int depth) override;
		TileMap::Layer& GetLayer(int depth) override;
		const TileMap::Layer& GetLayer(int depth) const override;
		bool RemoveLayer(int depth) override;

		const SizeI& TileSize() const override;
		void SetTileSize(const SizeI& value) override;

		void Resize(int rows, int columns) override;

		void Clear() override;

	private:
		TileMap tileMap;
		SizeI tileSize;
		TileMapRenderer renderer;

	};

}