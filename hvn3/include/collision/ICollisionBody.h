#pragma once
#include "collision/CollisionFilter.h"
#include "ICollidable.h"
#include "core/IPositionable2d.h"
#include "IHitMask.h"

namespace hvn3 {
	namespace Collision {

		class ICollisionBody : public IPositionable2d<float> {

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

			virtual bool IsSolid() const = 0;
			virtual void SetSolid(bool value) = 0;

		};

	}
}