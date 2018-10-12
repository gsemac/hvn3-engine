#pragma once
#include "hvn3/collision/ICollidable.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/core/IDestroyable.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	class CollisionFilter;
	class HitMask;
	class IObject;

	enum class ColliderFlags {
		Static,
		Solid
	};
	ENABLE_BITFLAG_OPERATORS(ColliderFlags);

	class ICollider :
		public IPositionable2d<float>,
		public IDestroyable {

	public:
		virtual ~ICollider() = default;

		// Returns the body's axis-aligned bounding box.
		virtual RectangleF AABB() const = 0;

		// Returns the collision filter associated with this body.
		virtual CollisionFilter& Filter() = 0;
		// Returns the collision filter associated with this body.
		virtual const CollisionFilter& Filter() const = 0;

		// Returns the hit mask associated with this body.
		virtual HitMask& HitMask() = 0;
		// Returns the hit mask associated with this body.
		virtual const class HitMask& HitMask() const = 0;
		// Sets the hit mask associated with this body.
		virtual void SetHitMask(const hvn3::HitMask& mask) = 0;

		virtual ColliderFlags Flags() const = 0;
		virtual void SetFlags(ColliderFlags flags) = 0;

		virtual void OnCollide(ICollider* other) const = 0;

	};

}