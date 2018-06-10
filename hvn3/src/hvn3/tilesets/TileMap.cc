#include "hvn3/exceptions/Exception.h"
#include "hvn3/tilesets/TileMap.h"
#include "hvn3/utility/BitUtils.h"
#include <iostream>

#define GET_TILE_INDEX(x, y) y * _width + x
#define TILE_INDEX_BITS 16u
#define AUTOTILE_INDEX_BITS 6u

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
	int TileMap::Count() const {
		return Width() * Height();
	}
	void TileMap::SetTile(int x, int y, tile_id tile) {
		SetTile(x, y, tile, 0);
	}
	void TileMap::SetTile(int x, int y, tile_id tile, layer_id depth) {

		// Add the layer if it does not already exist.
		AddLayer(depth);

		_layers[depth][GET_TILE_INDEX(x, y)] = tile;

	}
	TileMap::Tile TileMap::At(int x, int y) const {
		return At(x, y, 0);
	}
	TileMap::Tile TileMap::At(int x, int y, layer_id depth) const {
		return Tile(_getTileIdAt(x, y, depth));
	}
	bool TileMap::Exists(int x, int y, layer_id depth) const {

		// If the layer does not exist, the tile does not exist.
		auto it = _layers.find(depth);
		if (it == _layers.end())
			return false;

		// If the tile index is out of bounds, the tile does not exist.
		int index = GET_TILE_INDEX(x, y);
		if (index < 0 || static_cast<layer_map_type::size_type>(index) >= it->second.size())
			return false;

		// If the tile index is non-zero, the tile exists.
		return it->second[index] > 0;

	}
	bool TileMap::Exists(int x, int y, layer_id depth, tile_id tile_index) const {

		// If the layer does not exist, the tile does not exist.
		auto it = _layers.find(depth);
		if (it == _layers.end())
			return false;

		// If the tile index is out of bounds, the tile does not exist.
		int index = GET_TILE_INDEX(x, y);
		if (index < 0 || static_cast<layer_map_type::size_type>(index) >= it->second.size())
			return false;

		// Check whether the tile at this index has the same tile index.
		return Tile(it->second[index]).TileIndex() == tile_index;

	}
	void TileMap::AddLayer(int depth) {

		if (_layerExists(depth))
			return;

		_layers[depth] = std::vector<tile_id>();
		_layers[depth].resize(static_cast<size_t>(_width * _height), 0);

	}
	TileMap::layer_map_type::size_type TileMap::LayerCount() const {
		return _layers.size();
	}
	TileMap::layer_map_type::iterator TileMap::LayersBegin() {
		return _layers.begin();
	}
	TileMap::layer_map_type::iterator TileMap::LayersEnd() {
		return _layers.end();
	}
	TileMap::layer_map_type::const_iterator TileMap::LayersBegin() const {
		return _layers.begin();
	}
	TileMap::layer_map_type::const_iterator TileMap::LayersEnd() const {
		return _layers.end();
	}
	void TileMap::Clear() {
		_layers.clear();
	}
	void TileMap::Clear(layer_id depth) {

		auto it = _layers.find(depth);

		if (it == _layers.end())
			return;

		it->second.clear();

	}

	bool TileMap::_layerExists(layer_id layer) const {
		return _layers.count(layer) != 0;
	}
	TileMap::tile_id TileMap::_getTileIdAt(int x, int y, layer_id depth) const {

		int index = GET_TILE_INDEX(x, y);

		auto it = _layers.find(depth);
		if (it == _layers.end())
			throw System::ArgumentException("Tile depth is invalid.");

		if (index < 0 || static_cast<layer_map_type::size_type>(index) >= it->second.size())
			throw System::ArgumentException("Tile index is out of bounds.");

		return it->second[index];

	}




	TileMap::Tile::Tile(tile_id id) {
		this->id = id;
	}
	TileMap::tile_id TileMap::Tile::TileIndex() const {
		return id & 0xFFFF; // 16 bits (two bytes)
	}
	TileMap::tile_id TileMap::Tile::AutoTileIndex() const {
		return (id & 0x3F0000) >> 16; // 6 bits
	}
	void TileMap::Tile::SetTileIndex(tile_id value) {
		id |= value & 0xFFFF; // 16 bits
	}
	void TileMap::Tile::SetAutoTileIndex(tile_id value) {
		id = BitUtils::Copy(value, BitUtils::Count(value) - 6u, 6u, id, 10u);
	}

}