#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "CollisionGrid.h"
#include "Vector2d.h"
#include "Graphics.h"
#include "RoomBase.h"
#include "Exception.h"

namespace hvn3 {

	// Public members

	CollisionGrid::CollisionGrid(const Size<float>& cell_size) :
		_cell_size(cell_size) {
	}
	CollisionGrid::CollisionGrid(float cell_width, float cell_height) :
		CollisionGrid(Size<float>(cell_width, cell_height)) {
	}

	// Inherited from IBroadphase

	void CollisionGrid::AddCollider(ColliderType collider) {

		_colliders.push_back(collider);

	}
	void CollisionGrid::RemoveCollider(ColliderType collider) {

		_colliders.remove(collider);

	}
	void CollisionGrid::ClearColliders() {

		_grid.clear();

		_colliders.clear();

	}
	void CollisionGrid::Update() {

		// Clear existing map contents.
		_grid.clear();

		// Map all colliders to grid spaces.
		for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
			MapToCells(*it);

	}

	const CollisionGrid::ColliderPairCollection& CollisionGrid::FindPairs() {

		// Create a set for storing pairs (to prevent duplicates).
		static std::unordered_set<std::pair<Collider*, Collider*>, PairHasher> pairs;
		pairs.clear();

		// Interate over all cells, and for all other colliders in the same cell, generate a pair.
		for (auto i = _grid.begin(); i != _grid.end();) {

			// Determine the beginning and ending boundaries for the next key.
			auto starting_it = i;
			auto ending_it = i;
			while (ending_it != _grid.end() && ending_it->first == starting_it->first)
				++ending_it;

			// Generate colliding pairs for each combination of colliders with this key.
			for (auto j = starting_it; j != ending_it; ++j)
				for (auto k = std::next(j, 1); k != ending_it; ++k)
					pairs.insert(std::pair<Collider*, Collider*>(j->second, k->second));

			// Continue the loop for the next key.
			i = ending_it;

		}

		// Copy contents to vector.
		_pairs.clear();
		std::copy(pairs.begin(), pairs.end(), std::back_inserter(_pairs));

		// Return the result.
		return _pairs;

	}
	CollisionGrid::ColliderType CollisionGrid::Pick(const PointF& point) const {

		throw NotImplementedException();

	}
	void CollisionGrid::QueryRegion(const RectangleF& region, ColliderCollection& output, int filter) const {

		// get the cells that are occupied by the region.
		static std::vector<PointF> cells;
		cells.clear();
		GetIntersectedCells(region, cells);

		// Use a set to store the collider list (to prevent duplicates).
		static std::unordered_set<Collider*> colliders;
		colliders.clear();

		// Add all colliders in those cells to the output list.
		for (size_t i = 0; i < cells.size(); ++i) {
			auto r = _grid.equal_range(cells[i]);
			for (auto j = r.first; j != r.second; ++j)
				if (filter == 0 || j->second->Filter().CategoryBits() & filter)
					colliders.insert(j->second);
		}

		// Copy contents to vector.
		std::copy(colliders.begin(), colliders.end(), std::back_inserter(output));

	}
	Collider* CollisionGrid::QueryNearest(const PointF& point, int filter) const {

		// The algorithm will check a region surrounding the the given Point, and if no relevant colliders are found, the region will be expanded according by the cell size.
		// To potentially improve efficiency, the difference of the cells of the two regions could be checked instead.

		//ICollidable* closest = nullptr;
		//std::vector<Point> cells;
		//Rectangle region(point.X() - __cell_size.Width() / 2.0f, point.Y() - __cell_size.Height() / 2.0f, __cell_size.Width(), __cell_size.Height());
		//do {
		//	float shortest_sq = -1.0f;
		//	GetIntersectedCells(region, cells);
		//	for (size_t i = 0; i < cells.size(); ++i) {
		//		auto r = __grid.equal_range(cells[i]);
		//		for (auto j = r.first; j != r.second; ++j)
		//			if (filter == 0 || j->second->CollisionId() & filter) {
		//				float dist = PointDistanceSquared(point, j->second->AABB());
		//				if (dist < shortest_sq) {
		//					shortest_sq = dist;
		//					closest = j->second;
		//				}
		//			}
		//	}
		//	if (closest) 
		//		return closest;
		//	cells.clear();
		//} while (region.Size() < Game::Scene().Dimensions());
		return nullptr;
	}
	RayCastResult CollisionGrid::RayCast(const LineF& ray) const {

		throw NotImplementedException();

	}

	// Private members

	void CollisionGrid::GetIntersectedCells(Collider* collider, std::vector<PointF>& cells) const {
		
		// If the mask type undefined, do not assign the collider to any cells.
		if (!collider->HitMask()) 
			return;

		// Get the collider's AABB. 
		RectangleF aabb = collider->AABB();

		// Get cells intersected by the AABB.
		GetIntersectedCells(aabb, cells);

	}
	void CollisionGrid::GetIntersectedCells(const RectangleF& region, std::vector<PointF>& cells) const {

		// Determine the cells intersected by each corner.
		PointF tl = CellAt(region.X(), region.Y());
		PointF br = CellAt(region.X2() - 1.0f, region.Y2() - 1.0f);

		// Add all cells in-between to the collection.
		int cw = static_cast<int>(_cell_size.Width());
		int ch = static_cast<int>(_cell_size.Height());
		for (int i = tl.X() * cw; i <= br.X() * cw; i += cw)
			for (int j = tl.Y() * ch; j <= br.Y() * ch; j += ch)
				cells.push_back(CellAt(i, j));

	}
	void CollisionGrid::MapToCells(Collider* collider) {

		// Initialize vector for holding assigned cells (re-used for every call).
		static std::vector<PointF> cells;
		cells.clear();

		// Get cells for the current collider.
		GetIntersectedCells(collider, cells);

		// Add all cells to the map.
		for (size_t i = 0; i < cells.size(); ++i)
			_grid.insert(std::pair<PointF, Collider*>(cells[i], collider));

	}

	//Object* CollisionGrid::RayCast(const Line& ray) {
	//
	//	// Calculate the direction of the line.
	//	float dir = PointDirection(ray.First(), ray.Second());
	//
	//	// Walk the line, checking each new sector.
	//	Point cell(-1.0f, -1.0f);
	//	Point current_pos = ray.First();
	//	do {
	//		if (cell == CellAt(current_pos)) {
	//			current_pos = PointInDirection(current_pos, dir, 1.0f);
	//			continue;
	//		}
	//		cell = CellAt(current_pos);
	//		std::vector<Object*>& objects = ObjectsAt(cell.X(), cell.Y);
	//		// Find the best candidate in this cell.
	//		for (size_t i = 0; i < objects.size(); ++i) {
	//			return objects[0];
	//		}
	//		current_pos = PointInDirection(current_pos, dir, 1.0f);
	//	} while (current_pos.X() < ray.Second().X() && current_pos.Y() < ray.Second().Y);
	//
	//	return nullptr;
	//}

	// Private members

	PointF CollisionGrid::CellAt(const PointF& point) const {

		return CellAt(point.X(), point.Y());

	}
	PointF CollisionGrid::CellAt(float x, float y) const {

		return PointF((std::floor)(x / _cell_size.Width()), (std::floor)(y / _cell_size.Height()));

	}
	void CollisionGrid::Draw(int cell_dimensions) const {

		//for (size_t i = 0; i < Columns(); ++i)
		//	for (size_t j = 0; j < Rows(); ++j) {
		//		al_draw_rectangle(i * cell_dimensions, j * cell_dimensions, i * cell_dimensions + cell_dimensions, j * cell_dimensions + cell_dimensions,
		//			al_map_rgba_f(0, 0, 0, 0.3), 2);
		//		if (__grid[i][j].size() > 0)
		//			al_draw_rectangle(i * cell_dimensions + 2, j * cell_dimensions + 2, i * cell_dimensions + cell_dimensions - 2, j * cell_dimensions + cell_dimensions - 2,
		//				al_map_rgba_f(0.8, 0.8, 0.8, 0.8), 2);
		//	}

	}

}