#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/tiles/TileManager.h"

namespace hvn3 {

	TileManager::TileManager(const SizeI& room_size, const SizeI& tile_size) :
		TileMap(static_cast<int>(Math::Ceiling(static_cast<float>(room_size.width) / tile_size.width)),
			static_cast<int>(Math::Ceiling(static_cast<float>(room_size.height) / tile_size.height))),
		TileMapRenderer(tile_size),
		_room_size(room_size) {
	}
	void TileManager::OnDraw(DrawEventArgs& e) {
		Draw(e.Graphics(), *this);
	}
	void TileManager::SetTileSize(const SizeI& tile_size) {

		TileMapRenderer::SetTileSize(tile_size);
		TileMap::Resize(static_cast<int>(Math::Ceiling(static_cast<float>(_room_size.width) / tile_size.width)),
			static_cast<int>(Math::Ceiling(static_cast<float>(_room_size.height) / tile_size.height)));

	}

}