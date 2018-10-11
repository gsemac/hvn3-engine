#pragma once
#include "hvn3/collision/IBroadPhase.h"

#include <unordered_map>
#include <vector>

namespace hvn3 {

	class SpatialPartitioningGrid :
		public IBroadPhase {

	public:
		SpatialPartitioningGrid(const SizeI& cell_size);
		SpatialPartitioningGrid(int cell_width, int cell_height);

		void Add(const IColliderPtr& body) override;
		size_t Count() const override;
		void Clear() override;

		void OnUpdate(UpdateEventArgs& e) override;

		const collider_pair_vector_type& FindCandidatePairs() override;

		void QueryRegion(const RectangleF& region, collider_vector_type& output, int filter = 0) const override;
		ICollider* QueryNearest(const PointF& point, int filter = 0) const override;
		RayCastResult RayCast(const LineF& ray, int mask = 0) const;

		CollisionManifold Pick(const PointF& point) const override;

	private:
		std::vector<IColliderPtr> _colliders;
		std::unordered_multimap<PointI, ICollider*> _grid;
		collider_pair_vector_type _pairs;
		SizeI _cell_size;

		// Used for temporarily holding the resulting cells from the "MapToCells" function. Avoids having to create a new vector each time.
		std::vector<PointI> _cell_mapping_buffer;

		void _getIntersectedCells(ICollider* body, std::vector<PointI>& cells) const;
		void _getIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const;
		void _mapToCells(ICollider* body);

		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI _cellAt(const PointF& point) const;
		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI _cellAt(float x, float y) const;

		void _clearDestroyedBodies();

	};

}