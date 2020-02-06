#include "hvn3/exceptions/Exception.h"
#include "hvn3/tiles/tile_map.h"
#include "hvn3/utility/Algorithm.h"
#include "hvn3/utility/BitUtils.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace hvn3::tiles {

	// Helper

	int GetTileIndex(int x, int y, int rows) {

		assert(x >= 0);
		assert(y >= 0);

		return y * rows + x;

	}

	// Public members

	TileMap::Tile::Tile() :
		Tile(0) {
	}
	TileMap::Tile::Tile(tile_id id) :
		id(id) {
	}

	TileMap::Tile::tile_id TileMap::Tile::TileIndex() const {

		return id & 0xFFFF; // 16 bits (two bytes)

	}
	TileMap::Tile::tile_id TileMap::Tile::AutoTileIndex() const {

		return (id & 0x3F0000) >> 16; // 6 bits

	}
	void TileMap::Tile::SetTileIndex(tile_id value) {

		id |= value & 0xFFFF; // 16 bits

	}
	void TileMap::Tile::SetAutoTileIndex(tile_id value) {

		id = BitUtils::Copy(value, BitUtils::Count(value) - 6u, 6u, id, 10u);

	}

	TileMap::Layer::Layer(int rows, int columns, int depth) :
		rows(rows),
		depth(depth) {

		assert(rows > 0);
		assert(columns > 0);

		tiles.resize(static_cast<size_t>(rows * columns));

	}

	void TileMap::Layer::SetTile(int x, int y, const Tile& tile) {

		int index = GetTileIndex(x, y, rows);

		assert(index >= 0);
		assert(static_cast<size_t>(index) < tiles.size());

		tiles[index] = tile;

	}
	TileMap::Tile& TileMap::Layer::GetTile(int x, int y) {

		int index = GetTileIndex(x, y, rows);

		assert(index >= 0);
		assert(static_cast<size_t>(index) < tiles.size());

		return tiles[index];

	}
	const TileMap::Tile& TileMap::Layer::GetTile(int x, int y) const {

		int index = GetTileIndex(x, y, rows);

		assert(index >= 0);
		assert(static_cast<size_t>(index) < tiles.size());

		return tiles[index];

	}
	bool TileMap::Layer::CheckTile(int x, int y) const {

		if (x < 0 || y < 0)
			return false;

		int index = GetTileIndex(x, y, rows);

		if (index < 0 || static_cast<size_t>(index) >= tiles.size())
			return false;

		return tiles[index].id != 0;

	}
	bool TileMap::Layer::CheckTile(int x, int y, const Tile& tile) const {

		if (x < 0 || y < 0)
			return false;

		int index = GetTileIndex(x, y, rows);

		if (index < 0 || static_cast<size_t>(index) >= tiles.size())
			return false;

		return tiles[index].id == tile.id;

	}
	int TileMap::Layer::Rows() const {

		return rows;

	}
	int TileMap::Layer::Columns() const {

		size_t columns = tiles.size() / static_cast<size_t>(rows);

		assert(columns <= static_cast<size_t>(std::numeric_limits<int>::max()));

		return static_cast<int>(columns);

	}
	int TileMap::Layer::Depth() const {

		return depth;

	}
	void TileMap::Layer::Clear() {

		std::fill(tiles.begin(), tiles.end(), Tile());

	}

	bool TileMap::Layer::operator<(const Layer& rhs) const {

		return Depth() < rhs.Depth();

	}
	bool TileMap::Layer::operator>(const Layer& rhs) const {

		return Depth() > rhs.Depth();

	}

	TileMap::TileMap(int rows, int columns) :
		rows(rows),
		columns(columns) {
	}

	int TileMap::Columns() const {

		return columns;

	}
	int TileMap::Rows() const {

		return rows;

	}
	TileMap::Layer& TileMap::AddLayer(int depth) {

		auto it = EmplaceSorted(layers, Layer(rows, columns, depth));

		return *it;

	}
	TileMap::Layer& TileMap::GetLayer(int depth) {

		auto it = std::find_if(layers.begin(), layers.end(),
			[=](const Layer& layer) {
				return layer.Depth() == depth;
			});

		assert(it != layers.end());

		return *it;

	}
	const TileMap::Layer& TileMap::GetLayer(int depth) const {

		auto it = std::find_if(layers.begin(), layers.end(),
			[=](const Layer& layer) {
				return layer.Depth() == depth;
			});

		assert(it != layers.end());

		return *it;

	}
	bool TileMap::RemoveLayer(int depth) {

		auto it = std::remove_if(layers.begin(), layers.end(),
			[=](const Layer& layer) {
				return layer.Depth() == depth;
			});

		if (it == layers.end())
			return false;

		layers.erase(it);

		return true;

	}
	const std::vector<TileMap::Layer>& TileMap::Layers() const {

		return layers;

	}

	void TileMap::SetTile(int x, int y, int depth, const TileMap::Tile& tile) {

		GetLayer(depth).SetTile(x, y, tile);

	}
	TileMap::Tile& TileMap::GetTile(int x, int y, int depth) {

		return GetLayer(depth).GetTile(x, y);

	}
	const TileMap::Tile& TileMap::GetTile(int x, int y, int depth) const {

		return GetLayer(depth).GetTile(x, y);

	}
	bool TileMap::CheckTile(int x, int y, int depth) const {

		return GetLayer(depth).CheckTile(x, y);

	}
	bool TileMap::CheckTile(int x, int y, int depth, const TileMap::Tile& tile) const {

		return GetLayer(depth).CheckTile(x, y, tile);

	}

	void TileMap::Resize(int rows, int columns) {

		if (rows == Rows() && columns == Columns())
			return;

		for (auto i = layers.begin(); i != layers.end(); ++i) {

			Layer newLayer(rows, columns, i->Depth());

			for (int y = 0; y < this->columns; ++y)
				for (int x = 0; x < this->rows; ++x)
					newLayer.SetTile(x, y, i->GetTile(x, y));

			*i = newLayer;

		}

		this->rows = rows;
		this->columns = columns;

	}
	void TileMap::Clear() {

		layers.clear();

	}

	/*void TileMap::GetNeighborsOf(std::vector<Tile>& out, int index, layer_id depth) const {

		PointI pos = _indexToPos(index);

		GetNeighborsOf(out, pos.x, pos.y, depth);

	}
	void TileMap::GetNeighborsOf(std::vector<Tile>& out, int x, int y, layer_id depth) const {

		PointI pos(x, y);

		PointI left(pos.x - 1, pos.y);
		PointI top_left(pos.x - 1, pos.y - 1);
		PointI top(pos.x, pos.y - 1);
		PointI top_right(pos.x + 1, pos.y - 1);
		PointI right(pos.x + 1, pos.y);
		PointI bottom_right(pos.x + 1, pos.y + 1);
		PointI bottom(pos.x, pos.y + 1);
		PointI bottom_left(pos.x - 1, pos.y + 1);

		for (PointI p : {left, top_left, top, top_right, right, bottom_right, bottom, bottom_left})
			out.push_back(At(p.x, p.y, depth));

	}*/

}