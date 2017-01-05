#pragma once
#include <vector>
#include <utility>
#include <unordered_set>
#include "Geometry.h"
#include "ICollidable.h"

struct RayCastResult {
	ICollidable* Hit;
	Point HitPosition;
};

class IBroadphase {

public:
	// Adds a new collider to the broadphase.
	virtual void Add(ICollidable* collider) = 0;

	// Removes a collider from the broadphase.
	virtual void Remove(ICollidable* collider) = 0;

	// Clears all colliders from the broadphase.
	virtual void Clear() = 0;

	// Updates broadphase to react to collider state changes.
	virtual void Update() = 0;

	// Returns a vector of potentially-colliding pairs.
	virtual const std::vector<std::pair<ICollidable*, ICollidable*>>& FindPairs() = 0;

	// Returns the first collider that collides with the given point, or null if none.
	//virtual ICollidable* Pick(const Point& point) const = 0;

	// Generates a vector of colliders that collide with the given region.
	virtual void QueryRegion(const Rectangle& region, std::vector<ICollidable*>& output, int filter = 0) const = 0;

	// Returns the nearest collider to the given point, or nullptr if no colliders found.
	virtual ICollidable* QueryNearest(const Point& point, int filter = 0) const = 0;

	// Returns the first collider that the ray hits, or null if none.
	//virtual RayCastResult RayCast(const Line& ray) const = 0;

};