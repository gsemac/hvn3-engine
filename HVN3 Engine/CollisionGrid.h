#pragma once
#include <unordered_map>
#include <list>
#include "Collider.h"
#include "IBroadPhaseCollisionManager.h"

namespace hvn3 {

	class CollisionGrid : public IBroadPhaseCollisionManager {

	public:
		CollisionGrid(float cell_width, float cell_height);
		CollisionGrid(const Size<float>& cell_size);

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

		//void HandleCollisions(Object* obj);
		//bool TestCollision(Object* obj, Object* other) const;
		//bool TestCollision(ICollidable* obj, Point obj_position, ICollidable* other, Point other_position) const;

		//std::vector<Object*>& ObjectsAt(float x, float y);
		PointF CellAt(const PointF& point) const;
		PointF CellAt(float x, float y) const;

		//Object* RayCast(const Line& ray);

	public:
		std::list<Collider*> _colliders;
		std::unordered_multimap<PointF, Collider*> _grid;
		std::vector<std::pair<Collider*, Collider*>> _pairs;
		Size<float> _cell_size;

		void GetIntersectedCells(Collider* collider, std::vector<PointF>& cells) const;
		void GetIntersectedCells(const RectangleF& region, std::vector<PointF>& cells) const;
		void MapToCells(Collider* collider);
		void Draw(int cell_dimensions) const;

	};

}