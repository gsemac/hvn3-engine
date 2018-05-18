#include "hvn3/exceptions/Exception.h"
#include "hvn3/tilesets/TileMap.h"

#define GET_TILE_INDEX(x, y) y * _width + x

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

		_tiles[depth][GET_TILE_INDEX(x, y)] = tile;

	}
	TileMap::Tile TileMap::At(int x, int y) const {
		return At(x, y, 0);
	}
	TileMap::Tile TileMap::At(int x, int y, layer_id depth) const {

		int index = GET_TILE_INDEX(x, y);

		auto it = _tiles.find(depth);
		if (it == _tiles.end())
			throw System::ArgumentException("Tile depth is invalid.");

		if (index < 0 || static_cast<layer_map_type::size_type>(index) >= it->second.size())
			throw System::ArgumentException("Tile index is out of bounds.");
		
		return Tile(it->second[index]);

	}
	void TileMap::AddLayer(int depth) {

		if (_layerExists(depth))
			return;

		_tiles[depth] = std::vector<tile_id>();
		_tiles[depth].resize(static_cast<size_t>(_width * _height), 0);

	}
	TileMap::layer_map_type::size_type TileMap::LayerCount() const {
		return _tiles.size();
	}
	TileMap::layer_map_type::iterator TileMap::LayersBegin() {
		return _tiles.begin();
	}
	TileMap::layer_map_type::iterator TileMap::LayersEnd() {
		return _tiles.end();
	}
	TileMap::layer_map_type::const_iterator TileMap::LayersBegin() const {
		return _tiles.begin();
	}
	TileMap::layer_map_type::const_iterator TileMap::LayersEnd() const {
		return _tiles.end();
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
		return _tiles.count(layer) != 0;
	}



	TileMap::Tile::Tile(tile_id id) {
		this->id = id;
	}

}