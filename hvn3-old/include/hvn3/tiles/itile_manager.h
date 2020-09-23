#pragma once

#include "hvn3/tiles/tile_map.h"
#include "hvn3/tiles/tileset.h"

namespace hvn3::tiles {

	class ITileManager {

	public:
		~ITileManager() = default;

		virtual void AddTileset(const Tileset& tileset) = 0;

		virtual TileMap::Layer& AddLayer(int depth) = 0;
		virtual TileMap::Layer& AddLayer(int rows, int columns, int depth) = 0;
		virtual TileMap::Layer& GetLayer(int depth) = 0;
		virtual const TileMap::Layer& GetLayer(int depth) const = 0;
		virtual bool RemoveLayer(int depth) = 0;

		virtual const SizeI& TileSize() const = 0;
		virtual void SetTileSize(const SizeI& value) = 0;

		virtual void Resize(int rows, int columns) = 0;

		virtual void Clear() = 0;

	};

}