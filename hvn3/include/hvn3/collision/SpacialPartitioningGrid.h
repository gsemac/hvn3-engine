#pragma once
#include "hvn3/collision/IBroadPhase.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/utility/Algorithm.h"
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <iterator>

namespace hvn3 {

	template <unsigned int cell_width, unsigned int cell_height>
	class SpacialPartitioningGrid final : public IBroadPhase {

	public:
		SpacialPartitioningGrid() :
			_cell_size(cell_width, cell_height) {
		}

		void AddBody(ICollisionBody& body) override {

			_colliders.push_back(&body);

		}
		void RemoveBody(ICollisionBody& body) override {

			_pending_removal.push_back(&body);

		}
		size_t Count() const override {

			return _colliders.size();

		}
		void Clear() override {

			_grid.clear();
			_colliders.clear();

		}

		void OnUpdate(UpdateEventArgs& e) override {

			// If there are any colliders pending removal, remove them now.
			if (_pending_removal.size() > 0) {
				_colliders.erase(RemoveSame(_colliders.begin(), _colliders.end(), _pending_removal.begin(), _pending_removal.end()).first, _colliders.end());
				_pending_removal.clear();
			}

			// Clear existing map contents.
			_grid.clear();

			// Map all colliders to grid spaces.
			for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
				_mapToCells(*it);

		}
		const collider_pair_vector_type& FindCandidatePairs() override {

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
					for (auto k = std::next(j, 1); k != ending_it; ++k) {

						// If the objects can't possibly collide due to their filter flags, skip the pair.
						if (
							!(j->second->Category().MaskBits() & k->second->Category().CategoryBits()) &&
							!(k->second->Category().MaskBits() & j->second->Category().CategoryBits())
							)
							continue;

						pairs.insert(std::pair<collider_type*, collider_type*>(j->second, k->second));

					}

				// Continue the loop for the next key.
				i = ending_it;

			}

			// Copy contents to vector.
			_pairs.clear();
			std::copy(pairs.begin(), pairs.end(), std::back_inserter(_pairs));

			// Return the result.
			return _pairs;

		}
		collider_type* Pick(const PointF& point) const override {

			throw System::NotImplementedException();

		}
		void QueryRegion(const RectangleF& region, collider_vector_type& output, int filter = 0) const override {

			// get the cells that are occupied by the region.
			static std::vector<PointI> cells;
			cells.clear();
			_getIntersectedCells(region, cells);

			// Use a set to store the collider list (to prevent duplicates).
			static std::unordered_set<collider_type*> colliders;
			colliders.clear();

			// Add all colliders in those cells to the output list.
			for (size_t i = 0; i < cells.size(); ++i) {
				auto r = _grid.equal_range(cells[i]);
				for (auto j = r.first; j != r.second; ++j)
					if (filter == 0 || j->second->Category().CategoryBits() & filter)
						colliders.insert(j->second);
			}

			// Copy contents to vector.
			std::copy(colliders.begin(), colliders.end(), std::back_inserter(output));

		}
		collider_type* QueryNearest(const PointF& point, int filter = 0) const override {

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
		RayCastResult RayCast(const LineF& ray) const {

			throw System::NotImplementedException();

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

		}

	private:
		std::vector<collider_type*> _colliders;
		std::vector<collider_type*> _pending_removal;
		std::unordered_multimap<PointI, collider_type*> _grid;
		collider_pair_vector_type _pairs;
		SizeI _cell_size;
		// Used for temporarily holding the resulting cells from the "MapToCells" function. Avoids having to create a new vector each time.
		std::vector<PointI> _cell_mapping_buffer;

		void _getIntersectedCells(collider_type* body, std::vector<PointI>& cells) const {

			// If the mask type undefined, do not assign the collider to any cells.
			if (!body->HitMask())
				return;

			// Get the collider's AABB. 
			RectangleF aabb = body->AABB();

			// Get cells intersected by the AABB.
			_getIntersectedCells(aabb, cells);

		}
		void _getIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const {

			// Determine the cells intersected by each corner.
			PointI tl = _cellAt(region.X(), region.Y());
			PointI br = _cellAt(region.X2(), region.Y2());
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
		void _mapToCells(collider_type* body) {

			// Clear the cell mapping buffer.
			_cell_mapping_buffer.clear();

			// Get all cells intersected by the given collider.
			_getIntersectedCells(body, _cell_mapping_buffer);

			// Add all cells to the map.
			for (size_t i = 0; i < _cell_mapping_buffer.size(); ++i)
				_grid.insert(std::pair<PointI, collider_type*>(_cell_mapping_buffer[i], body));

		}

		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI _cellAt(const PointF& point) const {

			return _cellAt(point.X(), point.Y());

		}
		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI _cellAt(float x, float y) const {

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

	};

}