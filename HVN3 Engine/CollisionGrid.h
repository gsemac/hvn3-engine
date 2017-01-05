#pragma once
#include <unordered_map>
#include <list>
#include "ICollidable.h"
#include "IBroadphase.h"

class CollisionGrid : public IBroadphase {

public:
	CollisionGrid(float cell_width, float cell_height);
	CollisionGrid(const Size& cell_size);
	
	// Inherited from IBroadphase

	void Add(ICollidable* collider) override;
	void Remove(ICollidable* collider) override;
	void Clear() override;
	void Update() override;
	const std::vector<std::pair<ICollidable*, ICollidable*>>& FindPairs() override;
	
	void QueryRegion(const Rectangle& region, std::vector<ICollidable*>& output, int filter = 0) const override;
	ICollidable* QueryNearest(const Point& point, int filter = 0) const override;

	//void Clear();

	//void HandleCollisions(Object* obj);
	//bool TestCollision(Object* obj, Object* other) const;
	//bool TestCollision(ICollidable* obj, Point obj_position, ICollidable* other, Point other_position) const;

	//std::vector<Object*>& ObjectsAt(float x, float y);
	Point CellAt(const Point& point) const;
	Point CellAt(float x, float y) const;

	//Object* RayCast(const Line& ray);

public:
	std::list<ICollidable*> __colliders;
	std::unordered_multimap<Point, ICollidable*> __grid;
	std::vector<std::pair<ICollidable*, ICollidable*>> __pairs;
	Size __cell_size;

	void GetIntersectedCells(ICollidable* collider, std::vector<Point>& cells) const;
	void GetIntersectedCells(const Rectangle& region, std::vector<Point>& cells) const;
	void MapToCells(ICollidable* collider);
	void Draw(int cell_dimensions) const;

};