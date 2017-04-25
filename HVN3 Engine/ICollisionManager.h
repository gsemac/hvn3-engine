#pragma once

class ICollidable;
class UpdateEventArgs;

class ICollisionManager {

public:
	virtual void ColliderAdd(ICollidable* collider) = 0;
	virtual void ColliderRemove(ICollidable* collider) = 0;
	virtual void Clear() = 0;

	// Returns true if the collider collides with any other collider at the given position.
	virtual bool PlaceFree(ICollidable* collider, float x, float y) = 0;
	// Moves the object a set distance in a given direction (in degrees) until it collides with another collider.
	virtual void MoveContact(ICollidable* collider, float direction, int max_distance) = 0;
	
	virtual void Update(UpdateEventArgs& e) = 0;

};