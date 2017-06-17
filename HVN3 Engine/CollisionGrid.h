#pragma once
#include <unordered_map>
#include <list>
#include "Collider.h"
#include "IBroadPhaseCollisionManager.h"
#include "DrawEventArgs.h"

namespace hvn3 {

	class CollisionGrid : public IBroadPhaseCollisionManager {

	public:
		CollisionGrid(int cell_width, int cell_height);
		CollisionGrid(const SizeI& cell_size);

		// Inherited from IBroadphase

		void AddCollider(ColliderType collider) override;
		void RemoveCollider(ColliderType collider) override;
		void ClearColliders() override;

		void Update() override;

		const ColliderPairCollection& FindPairs() override;
		ColliderType Pick(const PointF& point) const override;
		void QueryRegion(const RectangleF& region, ColliderCollection& output, int filter = 0) const override;
		Collider* QueryNearest(const PointF& point, int filter = 0) const override;
		RayCastResult RayCast(const LineF& ray) const;

#ifdef HVN3_DEBUG
		void DrawCells(DrawEventArgs& e) const;
#endif

	private:
		void GetIntersectedCells(ColliderType collider, std::vector<PointI>& cells) const;
		void GetIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const;
		void MapToCells(ColliderType collider);

		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI CellAt(const PointF& point) const;
		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI CellAt(float x, float y) const;

		std::list<ColliderType> _colliders;
		std::unordered_multimap<PointI, ColliderType> _grid;
		ColliderPairCollection _pairs;
		SizeI _cell_size;
		// Used for temporarily holding the resulting cells from the "MapToCells" function. Avoids having to create a new vector each time.
		std::vector<PointI> _cell_mapping_buffer;



	};

}