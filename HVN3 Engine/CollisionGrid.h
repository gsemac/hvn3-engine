#pragma once
#include <unordered_map>
#include <list>
#include "IBroadPhaseCollisionManager.h"
#include "DrawEventArgs.h"

namespace hvn3 {

	class CollisionGrid final : public IBroadPhaseCollisionManager {

	public:
		CollisionGrid(int cell_width, int cell_height);
		CollisionGrid(const SizeI& cell_size);

		void AddBody(ICollisionBody* body) override;
		bool RemoveBody(ICollisionBody* body) override;
		void ClearAll() override;
		void OnUpdate(UpdateEventArgs& e) override;
		const collider_pair_collection_type& FindCandidatePairs() override;
		collider_type* Pick(const PointF& point) const override;
		void QueryRegion(const RectangleF& region, collider_collection_type& output, int filter = 0) const override;
		collider_type* QueryNearest(const PointF& point, int filter = 0) const override;
		RayCastResult RayCast(const LineF& ray) const;

#ifdef HVN3_DEBUG
		void DrawCells(DrawEventArgs& e) const;
#endif

	private:
		void GetIntersectedCells(collider_type* body, std::vector<PointI>& cells) const;
		void GetIntersectedCells(const RectangleF& region, std::vector<PointI>& cells) const;
		void MapToCells(collider_type* body);

		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI CellAt(const PointF& point) const;
		// Given a position, returns the position of the cell that contains it, aligned to the grid.
		PointI CellAt(float x, float y) const;

		std::list<collider_type*> _colliders;
		std::unordered_multimap<PointI, collider_type*> _grid;
		collider_pair_collection_type _pairs;
		SizeI _cell_size;
		// Used for temporarily holding the resulting cells from the "MapToCells" function. Avoids having to create a new vector each time.
		std::vector<PointI> _cell_mapping_buffer;



	};

}