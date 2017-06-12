#pragma once
#include <unordered_map>
#include <list>
#include "ICollidable.h"
#include "IBroadPhaseCollisionManager.h"

namespace hvn3 {

	class CollisionGrid : public IBroadPhaseCollisionManager {

	public:
		CollisionGrid(float cell_width, float cell_height);
		CollisionGrid(const Size<float>& cell_size);

		// Inherited from IBroadphase

		void Add(ICollidable* collider) override;
		void Remove(ICollidable* collider) override;
		void Clear() override;
		void Update() override;
		const std::vector<std::pair<ICollidable*, ICollidable*>>& FindPairs() override;

		void QueryRegion(const Rectangle<float>& region, std::vector<ICollidable*>& output, int filter = 0) const override;
		ICollidable* QueryNearest(const Point2d<float>& point, int filter = 0) const override;

		//void Clear();

		//void HandleCollisions(Object* obj);
		//bool TestCollision(Object* obj, Object* other) const;
		//bool TestCollision(ICollidable* obj, Point obj_position, ICollidable* other, Point other_position) const;

		//std::vector<Object*>& ObjectsAt(float x, float y);
		Point2d<float> CellAt(const Point2d<float>& point) const;
		Point2d<float> CellAt(float x, float y) const;

		//Object* RayCast(const Line& ray);

	public:
		std::list<ICollidable*> __colliders;
		std::unordered_multimap<Point2d<float>, ICollidable*> __grid;
		std::vector<std::pair<ICollidable*, ICollidable*>> __pairs;
		Size<float> __cell_size;

		void GetIntersectedCells(ICollidable* collider, std::vector<Point2d<float>>& cells) const;
		void GetIntersectedCells(const Rectangle<float>& region, std::vector<Point2d<float>>& cells) const;
		void MapToCells(ICollidable* collider);
		void Draw(int cell_dimensions) const;

	};

}