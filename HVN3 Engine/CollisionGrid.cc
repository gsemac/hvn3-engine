#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "CollisionGrid.h"
#include "Vector2d.h"
#include "Graphics.h"
#include "RoomBase.h"
#include "Exception.h"
#include "DrawEventArgs.h"
#include "ICollisionBody.h"

namespace hvn3 {

	// Public members

	CollisionGrid::CollisionGrid(const SizeI& cell_size) :
		_cell_size(cell_size) {
	}
	CollisionGrid::CollisionGrid(int cell_width, int cell_height) :
		CollisionGrid(SizeI(cell_width, cell_height)) {
	}

	void CollisionGrid::AddBody(ICollisionBody* body) {

		_colliders.push_back(body);

	}
	bool CollisionGrid::RemoveBody(ICollisionBody* body) {

		auto it = std::find(_colliders.begin(), _colliders.end(), body);

		if (it == _colliders.end())
			return false;

		_colliders.erase(it);

		return true;

	}
	void CollisionGrid::ClearAll() {

		_grid.clear();
		_colliders.clear();

	}
	void CollisionGrid::OnUpdate(UpdateEventArgs& e) {

		// Clear existing map contents.
		_grid.clear();

		// Map all colliders to grid spaces.
		for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
			MapToCells(*it);

	}
	const CollisionGrid::collider_pair_collection_type& CollisionGrid::FindPairs() {
	
		// Create a set for storing pairs (to prevent duplicates).
		static std::unordered_set<std::pair<collider_type*, collider_type*>, PairHasher> pairs;
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
					pairs.insert(std::pair<collider_type*, collider_type*>(j->second, k->second));

			// Continue the loop for the next key.
			i = ending_it;

		}

		// Copy contents to vector.
		_pairs.clear();
		std::copy(pairs.begin(), pairs.end(), std::back_inserter(_pairs));

		// Return the result.
		return _pairs;

	}
	CollisionGrid::collider_type* CollisionGrid::Pick(const PointF& point) const {

		throw System::NotImplementedException();

	}
	void CollisionGrid::QueryRegion(const RectangleF& region, collider_collection_type& output, int filter) const {

		// get the cells that are occupied by the region.
		static std::vector<PointI> cells;
		cells.clear();
		GetIntersectedCells(region, cells);

		// Use a set to store the collider list (to prevent duplicates).
		static std::unordered_set<collider_type*> colliders;
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
	CollisionGrid::collider_type* CollisionGrid::QueryNearest(const PointF& point, int filter) const {

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

		throw System::NotImplementedException();

	}

	// Private members

	void CollisionGrid::GetIntersectedCells(collider_type* body, std::vector<PointI>& cells) const {

		// If the mask type undefined, do not assign the collider to any cells.
		if (!body->HitMask())
			return;

		// Get the collider's AABB. 
		RectangleF aabb = body->AABB();

		// Get cells intersected by the AABB.
		GetIntersectedCells(aabb, cells);

	}
	void CollisionGrid::GetIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const {

		// Determine the cells intersected by each corner.
		PointI tl = CellAt(region.X(), region.Y());
		PointI br = CellAt(region.X2(), region.Y2());
		RectangleI rect(tl, br);

		// Add all cells intersected by the resulting rectangle.
		for (int i = rect.X(); i <= rect.X2(); ++i)
			for (int j = rect.Y(); j <= rect.Y2(); ++j)
				cells.push_back(PointI(i, j));


		//	// Add all cells in-between to the collection.
		//	int cw = static_cast<int>(_cell_size.Width());
		//int ch = static_cast<int>(_cell_size.Height());
		//for (int i = tl.X() * cw; i <= br.X() * cw; i += cw)
		//	for (int j = tl.Y() * ch; j <= br.Y() * ch; j += ch)
		//		cells.push_back(CellAt(i, j));

	}
	void CollisionGrid::MapToCells(collider_type* body) {

		// Clear the cell mapping buffer.
		_cell_mapping_buffer.clear();

		// Get all cells intersected by the given collider.
		GetIntersectedCells(body, _cell_mapping_buffer);

		// Add all cells to the map.
		for (size_t i = 0; i < _cell_mapping_buffer.size(); ++i)
			_grid.insert(std::pair<PointI, collider_type*>(_cell_mapping_buffer[i], body));

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

	PointI CollisionGrid::CellAt(const PointF& point) const {

		return CellAt(point.X(), point.Y());

	}
	PointI CollisionGrid::CellAt(float x, float y) const {

		// Round the floating point position down (ex: [32.2, 6.2] should end up in cell [1, 0]).
		x = Math::Floor(x);
		y = Math::Floor(y);

		// Calculate the position of the cell, keeping in mind the potential for negative cell positions.
		PointI cell(
			static_cast<int>(x / 32 - (x < 0)),
			static_cast<int>(y / 32 - (y < 0))
			);

		// Return the result.
		return cell;

	}

	// Debug methods

#ifdef HVN3_DEBUG
	void CollisionGrid::DrawCells(DrawEventArgs& e) const {

		for (auto i = _grid.begin(); i != _grid.end(); ++i) {

			PointI cell = (*i).first;

			e.Graphics().DrawRectangle(
				static_cast<float>(cell.X() * _cell_size.Width()),
				static_cast<float>(cell.Y() * _cell_size.Width()),
				static_cast<float>(_cell_size.Width()),
				static_cast<float>(_cell_size.Height()),
				Color::Fuchsia, 1.0f);

		}

	}
#endif

}