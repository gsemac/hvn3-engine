#pragma once
#include <cstddef>
#include <map>
#include <vector>

namespace hvn3 {

	class TileMap {

	public:
		typedef uint32_t tile_id;
		typedef int layer_id;

		TileMap(int width, int height);

		int Width() const;
		int Height() const;

		void SetTile(int x, int y, tile_id tile);
		void SetTile(int x, int y, tile_id tile, layer_id depth);
		void AddLayer(int depth);

		void Clear();
		void Clear(layer_id depth);

	private:
		int _width, _height;
		std::map<layer_id, std::vector<tile_id>> _tiles;

		bool _layerExists(layer_id layer) const;

	};

}