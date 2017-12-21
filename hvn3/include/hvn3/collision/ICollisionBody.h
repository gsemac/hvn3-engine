#pragma once
#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/ICollidable.h"
#include "hvn3/collision/IHitMask.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/core/IDestroyable.h"

namespace hvn3 {

	class ICollisionManager;

	namespace System {
		class CollisionBodyMutator;
	}

	class ICollisionBody : public IPositionable2d<float>, public IDestroyable {

		friend class System::CollisionBodyMutator;

	public:
		virtual ~ICollisionBody() = default;

		// Returns the body's axis-aligned bounding box.
		virtual RectangleF AABB() const = 0;

		// Returns the collision filter associated with this body.
		virtual CategoryFilter& Category() = 0;
		// Returns the collision filter associated with this body.
		virtual const CategoryFilter& Category() const = 0;

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

	private:
		// Sets the collision manager currently managing this body.
		virtual void _setManager(ICollisionManager* manager) = 0;

	};

}