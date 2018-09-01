#pragma once
#include <cstddef>
#include <map>
#include <vector>

namespace hvn3 {

	class TileMap {

	public:
		typedef uint32_t tile_id;
		typedef int layer_id;
		typedef std::vector<tile_id> tile_vector_type;
		typedef std::map<layer_id, std::vector<tile_id>> layer_map_type;

		class Tile {

		public:
			Tile(tile_id id);

			tile_id TileIndex() const;
			tile_id AutoTileIndex() const;
			void SetTileIndex(tile_id value);
			void SetAutoTileIndex(tile_id value);

			tile_id id;

		};

		TileMap(int width, int height);

		int Columns() const;
		int Rows() const;
		int Count() const;

		void SetTile(int x, int y, tile_id tile);
		void SetTile(int x, int y, tile_id tile, layer_id depth);
		Tile AtIndex(int n) const;
		Tile AtIndex(int n, layer_id depth) const;
		Tile At(int x, int y) const;
		Tile At(int x, int y, layer_id depth) const;
		bool Exists(int x, int y, layer_id depth) const;
		bool Exists(int x, int y, layer_id depth, tile_id tile_index) const;

		void AddLayer(int depth);
		layer_map_type::size_type LayerCount() const;
		layer_map_type::iterator LayersBegin();
		layer_map_type::iterator LayersEnd();
		layer_map_type::const_iterator LayersBegin() const;
		layer_map_type::const_iterator LayersEnd() const;

		void Resize(int width, int height);
		void Clear();
		void Clear(layer_id depth);

	private:
		int _width, _height;
		layer_map_type _layers;

		bool _layerExists(layer_id layer) const;
		tile_id _getTileIdAt(int n, layer_id depth) const;
		tile_id _getTileIdAt(int x, int y, layer_id depth) const;

	};

}