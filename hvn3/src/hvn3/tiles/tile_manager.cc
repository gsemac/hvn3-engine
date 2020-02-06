#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/tiles/tile_manager.h"

namespace hvn3::tiles {

	// Public members

	TileManager::TileManager() :
		tileMap(0, 0),
		tileSize(32, 32) {
	}

	void TileManager::OnEvent(DrawEventArgs& e) {

		renderer.DrawTiles(e.Graphics(), tileMap, tileSize);

	}

	void TileManager::AddTileset(const Tileset& tileset) {

		if (tileSize.width <= 0 || tileSize.height <= 0)
			tileSize = tileset.TileSize();

		renderer.AddTileset(tileset);

	}

	TileMap::Layer& TileManager::AddLayer(int depth) {

		return tileMap.AddLayer(depth);

	}
	TileMap::Layer& TileManager::AddLayer(int rows, int columns, int depth) {

		Resize(rows, columns);

		return AddLayer(depth);

	}
	TileMap::Layer& TileManager::GetLayer(int depth) {

		return tileMap.GetLayer(depth);

	}
	const TileMap::Layer& TileManager::GetLayer(int depth) const {

		return tileMap.GetLayer(depth);

	}
	bool TileManager::RemoveLayer(int depth) {

		return tileMap.RemoveLayer(depth);

	}
	const SizeI& TileManager::TileSize() const {

		return tileSize;

	}
	void TileManager::SetTileSize(const SizeI& value) {

		assert(value.width > 0);
		assert(value.height > 0);

		tileSize = value;

	}
	void TileManager::Resize(int rows, int columns) {

		tileMap.Resize(rows, columns);

	}
	void TileManager::Clear() {

		tileMap.Clear();

		renderer = TileMapRenderer();

	}

}