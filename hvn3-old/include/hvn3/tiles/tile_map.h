#pragma once
#include "hvn3/math/Point2d.h"
#include <cstddef>
#include <map>
#include <vector>

namespace hvn3::tiles {

	class TileMap {

	public:

		class Tile {

		public:
			typedef uint32_t tile_id;

			Tile();
			Tile(tile_id id);

			tile_id TileIndex() const;
			tile_id AutoTileIndex() const;
			void SetTileIndex(tile_id value);
			void SetAutoTileIndex(tile_id value);

			tile_id id;

		};

		class Layer {

		public:
			Layer(int rows, int columns, int depth);

			void SetTile(int x, int y, const Tile& tile);
			Tile& GetTile(int x, int y);
			const Tile& GetTile(int x, int y) const;
			bool CheckTile(int x, int y) const;
			bool CheckTile(int x, int y, const Tile& tile) const;
			//void GetNeighborsOf(std::vector<Tile>& out, int index, layer_id depth) const;
			//void GetNeighborsOf(std::vector<Tile>& out, int x, int y, layer_id depth) const;

			int Rows() const;
			int Columns() const;
			int Depth() const;

			void Clear();

			bool operator<(const Layer& rhs) const;
			bool operator>(const Layer& rhs) const;

		private:
			int rows;
			int depth;
			std::vector<Tile> tiles;

		};

		TileMap(int rows, int columns);

		int Columns() const;
		int Rows() const;

		Layer& AddLayer(int depth);
		Layer& GetLayer(int depth);
		const Layer& GetLayer(int depth) const;
		bool RemoveLayer(int depth);

		void SetTile(int x, int y, int depth, const Tile& tile);
		Tile& GetTile(int x, int y, int depth);
		const Tile& GetTile(int x, int y, int depth) const;
		bool CheckTile(int x, int y, int depth) const;
		bool CheckTile(int x, int y, int depth, const Tile& tile) const;

		const std::vector<Layer>& Layers() const;

		void Resize(int rows, int columns);

		void Clear();

	private:
		int rows;
		int columns;
		std::vector<Layer> layers;

	};

}