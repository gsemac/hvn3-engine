#pragma once
#include "Point2d.h"
#include "IBroadPhaseCollisionManager.h"
#include "Vector2d.h"
#include <functional>

namespace hvn3 {

	struct CollisionManifold {
		ICollisionBody* BodyA;
		ICollisionBody* BodyB;
		Vector2d Normal;
		float Penetration;
	};
	
	class ICollisionBody;
		
	template <typename _key_type>
	class ICollisionManager : public IUpdatable {

	public:
		typedef _key_type key_type;

		// Adds a new body to the collision manager.
		virtual ICollisionBody* GetBody(key_type key) = 0;
		virtual const ICollisionBody* GetBody(key_type key) const = 0;
		virtual ICollisionBody* CreateBody(key_type key) = 0;
		// Removes the body from the manager with the given key. Returns true if a body was removed.
		virtual bool RemoveBody(key_type key) = 0;
		// Clears all bodies from the manager.
		virtual void ClearAll() = 0;

		virtual const std::vector<CollisionManifold>& CollidingPairs() const = 0;

		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(ICollisionBody* body, const PointF& position) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true.
		virtual bool PlaceFreeIf(ICollisionBody* body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body.
		virtual bool MoveContact(ICollisionBody* body, float direction, float max_distance) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true.
		virtual bool MoveContactIf(ICollisionBody* body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with any other bodies.
		virtual bool MoveOutside(ICollisionBody* body, float direction, float max_distance) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with the given body.
		virtual bool MoveOutsideBody(ICollisionBody* key, ICollisionBody* other, float direction, float max_distance) = 0;

		virtual IBroadPhaseCollisionManager* BroadPhase() = 0;

	};

}