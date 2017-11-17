#pragma once
#include "CollisionFilter.h"
#include "ICollidable.h"
#include "IHitMask.h"
#include "core/IPositionable2d.h"
#include "core/IDestroyable.h"

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;

	namespace System {
		class CollisionBodyMutator;
	}

	class ICollisionBody : public IPositionable2d<float>, public IDestroyable {

		friend class System::CollisionBodyMutator;

	public:
		// Returns the body's axis-aligned bounding box.
		virtual RectangleF AABB() const = 0;

		// Returns the collision filter associated with this body.
		virtual CollisionFilter& Filter() = 0;
		// Returns the collision filter associated with this body.
		virtual const CollisionFilter& Filter() const = 0;

		// Returns the hit mask associated with this body.
		virtual HitMaskPtr& HitMask() = 0;
		// Returns the hit mask associated with this body.
		virtual const HitMaskPtr& HitMask() const = 0;
		// Sets the hit mask associated with this body.
		virtual void SetHitMask(HitMaskPtr& mask) = 0;

		// Returns true if the solid flag is set.
		virtual bool IsSolid() const = 0;
		// Sets the solid flag to the given value.
		virtual void SetSolid(bool value) = 0;

		// Returns true if the body collides with any other body at the given position.
		virtual bool PlaceFree(const PointF& position) = 0;
		// Returns true if the body collides with any other body at the given position for which the given condition is true.
		virtual bool PlaceFreeIf(const PointF& position, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body.
		virtual bool MoveContact(float direction, float max_distance) = 0;
		// Moves the body a set distance in a given direction (in degrees) until it collides with another body for which the given condition is true.
		virtual bool MoveContactIf(float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with any other bodies.
		virtual bool MoveOutside(float direction, float max_distance) = 0;
		// Moves the body in a given direction (in degrees) until it is no longer colliding with the given body.
		virtual bool MoveOutsideBody(ICollisionBody* other, float direction, float max_distance) = 0;

	private:
		// Sets the broad phase and narrow phase managers pair use for detecting collisions involving this body.
		virtual void _setCollisionPhasePair(IBroadPhase* broad_phase, INarrowPhase* narrow_phase) = 0;

	};

}