#pragma once
#include "hvn3/collision/CollisionDefs.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/math/Point2d.h"

#include <functional>
#include <vector>

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;
	class ICollidable;
	class ICollider;
	struct CollisionResult;

	class ICollisionManager :
		public IUpdatable,
		public IManager {

	public:
		typedef std::function<bool(const ICollider*)> condition_lambda_type;

		virtual ~ICollisionManager() = default;

		// Adds a new collision body to the collision system.
		virtual void Add(const IColliderPtr& collider) = 0;
		// Returns the broad phase collision manager used by the manager.
		virtual const IBroadPhase& Broad() const = 0;
		// Returns the narrow phase collision manager used by the manager.
		virtual const INarrowPhase& Narrow() const = 0;
		// Returns a collection of all colliding pairs found in the last update.
		virtual const std::vector<CollisionResult>& CollidingPairs() const = 0;
		// Returns the number of collision bodies added to the manager.
		virtual size_t Count() const = 0;
		// Clears all collision bodies from the manager.
		virtual void Clear() = 0;

		// Returns the level of precision used for collision calculations, in pixels.
		virtual float Precision() const = 0;
		// Sets the level of precision used for collision calculations, in pixels.
		virtual void SetPrecision(float value) = 0;

		// Returns true if the collider does not collide with any other collider at its current position.
		virtual bool PlaceFree(ICollider& body) = 0;
		// Returns true if the collider does not collide with any other collider at its current position, and stores information about the collision (if applicable).
		virtual bool PlaceFree(ICollider& body, CollisionResult& manifold) = 0;
		// Returns true if the collider does not collide with any other collider at the given position.
		virtual bool PlaceFree(ICollider& body, const PointF& position) = 0;
		// Returns true if the collider does not collide with any other collider at the given position, and stores information about the collision (if applicable).
		virtual bool PlaceFree(ICollider& body, const PointF& position, CollisionResult& manifold) = 0;

		// Returns true if the collider collides with any other collider with the given category at its current position.
		virtual bool PlaceMeeting(ICollider& body, int category) = 0;
		// Returns true if the collider collides with any other collider with the given category at its current position, and stores information about the collision (if applicable).
		virtual bool PlaceMeeting(ICollider& body, int category, CollisionResult& manifold) = 0;
		// Returns true if the collider collides with any other collider with the given category at the given position.
		virtual bool PlaceMeeting(ICollider& body, const PointF& position, int category) = 0;
		// Returns true if the collider collides with any other collider with the given category at the given position, and stores information about the collision (if applicable).
		virtual bool PlaceMeeting(ICollider& body, const PointF& position, int category, CollisionResult& manifold) = 0;
		// Returns true if the collider collides with any other collider with the given category at the given position which meets the given condition, and stores information about the collision (if applicable).
		virtual bool PlaceMeeting(ICollider& body, const PointF& position, const condition_lambda_type& where, CollisionResult& manifold) = 0;

		// Moves the body a set distance in a given direction (in degrees) until it collides with another body.
		virtual bool MoveContact(ICollider* body, float direction, float distance) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with a body with the given category.
		virtual bool MoveContact(ICollider* body, float direction, float distance, int category) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true.
		virtual bool MoveContactIf(ICollider* body, float direction, float distance, const condition_lambda_type& condition) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true, and stores collision information.
		virtual bool MoveContactIf(ICollider* body, float direction, float distance, CollisionResult& manifold, const condition_lambda_type& condition) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with any other bodies.
		virtual bool MoveOutside(ICollider* body, float direction, float max_distance) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with the given body.
		virtual bool MoveOutsideBody(ICollider* body, ICollider* other, float direction, float max_distance) = 0;

	};

	template<>
	struct ManagerIdTraits<COLLISION_MANAGER> {
		typedef ICollisionManager type;
	};

}