#pragma once
#include "CollisionMask.h"
#include "CollisionFilter.h"

class RoomBase;

class ICollidable : public IPositionable {

public:
	ICollidable(float x, float y);

	Rectangle AABB() const;
	CollisionMask& CollisionMask();
	void SetCollisionMask(const ::CollisionMask& mask);
	CollisionFilter& Filter();

	//// Returns a pointer to the Scene the object belongs to, or nullptr if the object does not belong to a Scene.
	//Room& Scene();

	bool CollidesWith(const ICollidable* other) const;
	virtual void Collide(ICollidable* other);

private:
	::CollisionMask __mask;
	CollisionFilter __filter;
	//::Room* __scene;

};