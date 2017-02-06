#pragma once
#include "CollisionMask.h"
#include "CollisionFilter.h"

class Scene;

class ICollidable : public IPositionable {
	friend class Scene;

public:
	ICollidable(float x, float y);

	Rectangle AABB() const;
	CollisionMask& CollisionMask();
	void SetCollisionMask(const ::CollisionMask& mask);
	CollisionFilter& Filter();

	// Returns a pointer to the Scene the object belongs to, or nullptr if the object does not belong to a Scene.
	Scene& Scene();

	bool CollidesWith(const ICollidable* other) const;
	virtual void Collide(ICollidable* other);
	
	// Returns true if the object meets any objects that it is able to collide with at the given position.
	bool PlaceFree(float x, float y, bool notme = true);

	// Moves the object a set distance in a given direction (in degrees) until it comes in contact with an object in its collision filter.
	void MoveContact(float direction, int max_distance);

private:
	::CollisionMask __mask;
	CollisionFilter __filter;
	::Scene* __scene;
	
};