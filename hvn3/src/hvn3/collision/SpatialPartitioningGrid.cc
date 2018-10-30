#include "hvn3/collision/CollisionFilter.h"
#include "hvn3/collision/CollisionResult.h"
#include "hvn3/collision/IBroadPhase.h"
#include "hvn3/collision/ICollider.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/collision/SpatialPartitioningGrid.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/utility/Algorithm.h"
#include "hvn3/utility/Hash.h"

#include <cassert>
#include <memory>
#include <unordered_set>

namespace hvn3 {

	SpatialPartitioningGrid::SpatialPartitioningGrid(const SizeI& cell_size) :
		_cell_size(cell_size) {

		assert(cell_size.width > 0);
		assert(cell_size.height > 0);

	}
	SpatialPartitioningGrid::SpatialPartitioningGrid(int cell_width, int cell_height) :
		SpatialPartitioningGrid(SizeI(cell_width, cell_height)) {
	}
	void SpatialPartitioningGrid::Add(const IColliderPtr& body) {
		_colliders.push_back(body);
	}
	size_t SpatialPartitioningGrid::Count() const {
		return _colliders.size();
	}
	void SpatialPartitioningGrid::Clear() {

		_grid.clear();
		_colliders.clear();

	}
	void SpatialPartitioningGrid::OnUpdate(UpdateEventArgs& e) {

		bool clear_bodies_required = false;

		// Clear existing map contents.
		_grid.clear();

		// Map all colliders to grid spaces.
		for (auto it = _colliders.begin(); it != _colliders.end(); ++it) {

			if (it->use_count() <= 1)
				clear_bodies_required = true;
			else
				_mapToCells(it->get());

		}

		// If any dead bodies were found, remove them from the manager now.
		if (clear_bodies_required)
			_clearDestroyedBodies();

	}
	const SpatialPartitioningGrid::collider_pair_vector_type& SpatialPartitioningGrid::FindCandidatePairs() {

		_pairs.clear();

		// A hash set is used for storing pairs in order to prevent duplicates.
		std::unordered_set<std::pair<ICollider*, ICollider*>, PairHasher> pairs;

		// Iterate over all cells, and for all other colliders in the same cell, generate a pair.
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
						!(j->second->Filter().CheckMatch(k->second->Filter())) &&
						!(k->second->Filter().CheckMatch(j->second->Filter()))
						)
						continue;

					pairs.insert(std::make_pair(j->second, k->second));

				}

			// Continue the loop for the next key.
			i = ending_it;

		}

		// Copy the result to the output vector.
		std::copy(pairs.begin(), pairs.end(), std::back_inserter(_pairs));

		// Return the result.
		return _pairs;

	}
	void SpatialPartitioningGrid::QueryRegion(const RectangleF& region, collider_vector_type& output, int filter) const {

		// get the cells that are occupied by the region.
		static std::vector<PointI> cells;
		cells.clear();
		_getIntersectedCells(region, cells);

		// Use a set to store the collider list (to prevent duplicates).
		static std::unordered_set<ICollider*> colliders;
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
	/*void QueryLine(const LineF& line, collider_vector_type& output, int filter = 0) const override {

		std::unordered_set<ICollider*> hits;

	}*/
	ICollider* SpatialPartitioningGrid::QueryNearest(const PointF& point, int filter) const {

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

		throw System::NotImplementedException();

	}
	RayCastResult SpatialPartitioningGrid::RayCast(const LineF& ray, int mask) const {

		HitMask line_mask(ray);
		CollisionResult manifold;
		RayCastResult result;

		SupercoverLineAlgorithm(ray.First(), ray.Second(), static_cast<SizeF>(_cell_size), [&](int x, int y) {

			auto items = _grid.equal_range(PointI(x, y));
			float nearest_dist_sq = std::numeric_limits<float>::max();

			for (auto i = items.first; i != items.second; ++i) {

				if (mask != 0 && (i->second->Filter().CategoryBits() & mask) == 0)
					continue;

				if (!i->second->HitMask().TestCollisionAt(i->second->Position(), line_mask, manifold))
					continue;

				// #todo Instead of comparing distance to AABB, use distance to intersection point
				float dist_sq = Math::Geometry::PointDistanceSquared(ray.First(), manifold.position);

				if (dist_sq < nearest_dist_sq) {

					nearest_dist_sq = dist_sq;

					result.hit = i->second;
					result.position = manifold.position;

				}

			}

			if (result.hit != nullptr)
				HVN3_BREAK;

			HVN3_CONTINUE;

		});

		return result;

	}
	CollisionResult SpatialPartitioningGrid::Pick(const PointF& point) const {

		IBroadPhase::collider_vector_type hits;
		RectangleF mask(point.x, point.y, 1.0f, 1.0f);
		CollisionResult manifold;

		QueryRegion(mask, hits);

		for (auto i = hits.begin(); i != hits.end(); ++i) {

			PointF other_position = (*i)->Position();
			HitMask& other_mask = (*i)->HitMask();
			PointF other_offset = other_mask.Offset();

			other_mask.SetOffset({ other_mask.Offset().x + other_position.x, other_mask.Offset().y + other_position.y });

			bool result = (*i)->HitMask().TestCollision(mask, manifold);

			other_mask.SetOffset(other_offset);

			if (result) {

				manifold.collider = (*i);

				break;

			}

		}

		return manifold;

	}

	void SpatialPartitioningGrid::_getIntersectedCells(ICollider* body, std::vector<PointI>& cells) const {

		// If the mask type undefined, do not assign the collider to any cells.
		if (!body->HitMask())
			return;

		// Get the collider's AABB. 
		RectangleF aabb = body->AABB();

		// Get cells intersected by the AABB.
		_getIntersectedCells(aabb, cells);

	}
	void SpatialPartitioningGrid::_getIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const {

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
	void SpatialPartitioningGrid::_mapToCells(ICollider* body) {

		// Clear the cell mapping buffer.
		_cell_mapping_buffer.clear();

		// Get all cells intersected by the given collider.
		_getIntersectedCells(body, _cell_mapping_buffer);

		// Add all cells to the map.
		for (size_t i = 0; i < _cell_mapping_buffer.size(); ++i)
			_grid.insert(std::make_pair(_cell_mapping_buffer[i], body));

	}
	PointI SpatialPartitioningGrid::_cellAt(const PointF& point) const {

		return _cellAt(point.X(), point.Y());

	}
	PointI SpatialPartitioningGrid::_cellAt(float x, float y) const {

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
	void SpatialPartitioningGrid::_clearDestroyedBodies() {

		_colliders.erase(std::remove_if(_colliders.begin(), _colliders.end(), [](const IColliderPtr& body) {
			return body.use_count() <= 1;
		}), _colliders.end());

	}

}