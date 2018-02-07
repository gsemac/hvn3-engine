#pragma once
#include "hvn3/collision/CollisionBodyPtr.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Point2d.h"
#include <vector>
#include <functional>

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;
	class ICollidable;
	class ICollisionBody;
	struct CollisionManifold;

	template <typename collidable_type>
	class ICollisionManager : public IUpdatable {

	public:
		typedef collidable_type collidable_type;
		typedef collidable_type* collidable_ptr_type;
		typedef ICollisionBody collider_type;
		typedef collider_type* collider_ptr_type;
		typedef collider_ptr_type collider_handle_type;
		typedef std::function<bool(collider_handle_type)> condition_lambda_type;

		virtual ~ICollisionManager() = default;

		// Creates and adds a new body to the system, and returns a pointer to it. The body is owned by the collision manager.
		virtual collider_handle_type CreateBody(collidable_ptr_type collidable) = 0;
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

		// Returns the level of precision used for collision calculations, in pixels.
		virtual float Precision() const = 0;
		// Sets the level of precision used for collision calculations, in pixels.
		virtual void SetPrecision(float value) = 0;

		// Returns true if the body collides with any other body at its current position.
		virtual bool PlaceFree(collider_handle_type body) = 0;
		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(collider_handle_type body, const PointF& position) = 0;
		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(collider_handle_type body, float x, float y) = 0;
		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(collider_handle_type body, const PointF& position, CollisionManifold& manifold) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true.
		virtual bool PlaceFreeIf(collider_handle_type body, const PointF& position, const condition_lambda_type& condition) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true, and stores collision information.
		virtual bool PlaceFreeIf(collider_handle_type body, const PointF& position, CollisionManifold& manifold, const condition_lambda_type& condition) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body.
		virtual bool MoveContact(collider_handle_type body, float direction, float distance) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true.
		virtual bool MoveContactIf(collider_handle_type body, float direction, float distance, const condition_lambda_type& condition) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true, and stores collision information.
		virtual bool MoveContactIf(collider_handle_type body, float direction, float distance, CollisionManifold& manifold, const condition_lambda_type& condition) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with any other bodies.
		virtual bool MoveOutside(collider_handle_type body, float direction, float max_distance) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with the given body.
		virtual bool MoveOutsideBody(collider_handle_type body, collider_handle_type other, float direction, float max_distance) = 0;

	};

}