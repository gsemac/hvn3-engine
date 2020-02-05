#pragma once

#include "hvn3/tiles/tile_map.h"

namespace hvn3::tiles {

	class ITileManager {

	public:
		~ITileManager() = default;

		virtual TileMap::Layer& AddLayer(int depth) = 0;
		virtual TileMap::Layer& GetLayer(int depth) = 0;
		virtual const TileMap::Layer& GetLayer(int depth) const = 0;
		virtual bool RemoveLayer(int depth) = 0;

		virtual void Resize(int rows, int columns) = 0;

		virtual void Clear() = 0;

	};

}