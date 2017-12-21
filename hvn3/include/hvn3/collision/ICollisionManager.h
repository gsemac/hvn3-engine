#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Point2d.h"
#include <vector>
#include <functional>

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;
	class ICollisionBody;
	struct CollisionManifold;

	class ICollisionManager : public IUpdatable {

	public:
		virtual ~ICollisionManager() = default;

		// Adds a new body to the collision manager.
		virtual void AddBody(ICollisionBody& body) = 0;
		// Marks a body for removal from the collision manager.
		virtual void RemoveBody(ICollisionBody& body) = 0;
		// Returns the broad phase collision manager used by the manager.
		virtual IBroadPhase& BroadPhase() = 0;
		// Returns the narrow phase collision manager used by the manager.
		virtual INarrowPhase& NarrowPhase() = 0;
		// Returns a collection of all colliding pairs found in the last update.
		virtual const std::vector<CollisionManifold>& CollidingPairs() const = 0;
		// Returns the number of collision bodies added to the manager.
		virtual size_t Count() const = 0;
		// Clears all collision bodies from the manager.
		virtual void Clear() = 0;

		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(ICollisionBody& body, const PointF& position) = 0;
		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(ICollisionBody& body, float x, float y) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true.
		virtual bool PlaceFreeIf(ICollisionBody& body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true, and stores the collision manifold.
		virtual bool PlaceFreeIf(ICollisionBody& body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition, CollisionManifold& manifold) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body.
		virtual bool MoveContact(ICollisionBody& body, float direction, float max_distance) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true.
		virtual bool MoveContactIf(ICollisionBody& body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with any other bodies.
		virtual bool MoveOutside(ICollisionBody& body, float direction, float max_distance) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with the given body.
		virtual bool MoveOutsideBody(ICollisionBody& body, ICollisionBody& other, float direction, float max_distance) = 0;

	};

}