#include "hvn3/tilesets/TileMap.h"

namespace hvn3 {

	TileMap::TileMap(int width, int height) {

		_width = width;
		_height = height;

		AddLayer(0);

	}
	int TileMap::Width() const {
		return _width;
	}
	int TileMap::Height() const {
		return _height;
	}
	void TileMap::SetTile(int x, int y, tile_id tile) {
		SetTile(x, y, tile, 0);
	}
	void TileMap::SetTile(int x, int y, tile_id tile, layer_id depth) {
		
		// Add the layer if it does not already exist.
		AddLayer(depth);

		_tiles[depth][y * _width + x] = tile;

	}
	void TileMap::AddLayer(int depth) {

		if (_layerExists(depth))
			return;

		_tiles[depth] = std::vector<tile_id>();
		_tiles[depth].resize(static_cast<size_t>(_width * _height), 0);

	}
	void TileMap::Clear() {
		_tiles.clear();
	}
	void TileMap::Clear(layer_id depth) {

		auto it = _tiles.find(depth);

		if (it == _tiles.end())
			return;

		it->second.clear();

	}

	bool TileMap::_layerExists(layer_id layer) const {
		return _tiles.count[layer] != 0;
	}

}