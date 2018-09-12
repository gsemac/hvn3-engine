#pragma once
#include "hvn3/collision/ICollidable.h"
#include "hvn3/collision/IHitMask.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/core/IDestroyable.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	class CategoryFilter;
	class IObject;

	enum class ColliderFlags {
		Static,
		Solid
	};
	ENABLE_BITFLAG_OPERATORS(ColliderFlags)

	class ICollider :
		public IPositionable2d<float>,
		public IDestroyable {

	public:
		virtual ~ICollider() = default;

		// Returns the body's axis-aligned bounding box.
		virtual RectangleF AABB() const = 0;

		// Returns the collision filter associated with this body.
		virtual CategoryFilter& GetCategory() = 0;
		// Returns the collision filter associated with this body.
		virtual const CategoryFilter& Category() const = 0;

		// Returns the hit mask associated with this body.
		virtual HitMaskPtr& GetHitMask() = 0;
		// Returns the hit mask associated with this body.
		virtual const HitMaskPtr& HitMask() const = 0;
		// Sets the hit mask associated with this body.
		virtual void SetHitMask(HitMaskPtr& mask) = 0;

		virtual ColliderFlags Flags() const = 0;
		virtual void SetFlags(ColliderFlags flags) = 0;
		
		virtual void OnCollide(ICollider* other) const = 0;

	};

}