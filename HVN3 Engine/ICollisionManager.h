#pragma once
#include "Point2d.h"

namespace hvn3 {

	class Object;
	class UpdateEventArgs;
	class IBroadPhaseCollisionManager;

	class ICollisionManager {

	public:
		// Adds a new object to the collision manager.
		virtual void AddObject(Object* object) = 0;
		// Removes an object from the collision manager. If the object does not exist, nothing happens.
		virtual void RemoveObject(Object* object) = 0;
		// Clears all objects from the collision manager.
		virtual void ClearObjects() = 0;

		// Returns true if the object collides with any other object at the given position.
		virtual bool PlaceFree(Object* object, const PointF& position) = 0;
		// Moves the object a set distance in a given direction (in degrees) until it collides with another object.
		virtual void MoveContact(Object* object, float direction, int distance_per_step) = 0;
		// Moves the object in a given direction (in degrees) until it is no longer colliding with any other objects.
		virtual void MoveOutside(Object* object, float direction, int distance_per_step) = 0;
		// Moves the object in a given direction (in degrees) until it is no longer colliding with the given object.
		virtual void MoveOutsideObject(Object* object, Object* other, float direction, int distance_per_step) = 0;

		virtual IBroadPhaseCollisionManager& BroadPhase() = 0;

		// Updates the state of the collision engine and triggers any new collisions.
		virtual void Update(UpdateEventArgs& e) = 0;

	};

}