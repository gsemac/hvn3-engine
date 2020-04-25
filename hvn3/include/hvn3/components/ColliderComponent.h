#pragma once

#include "hvn3/ecs/ComponentBase.h"
#include "hvn3/collision/CollisionEventArgs.h"
#include "hvn3/collision/CollisionFilter.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/collision/ICollisionListener.h"
#include "hvn3/components/TransformComponent.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	enum class ColliderFlags {
		Static,
		Solid
	};
	HVN3_ENABLE_BITFLAGS(ColliderFlags);

	class ColliderComponent :
		public ecs::ComponentBase<TypeList<TransformComponent>> {

	public:

		ColliderComponent();

		// Returns the body's axis-aligned bounding box.
		RectangleF Aabb() const;

		// Returns the collision filter associated with this body.
		CollisionFilter& GetFilter();
		// Returns the collision filter associated with this body.
		const CollisionFilter& Filter() const;

		// Returns the hit mask associated with this body.
		HitMask& GetHitMask();
		// Returns the hit mask associated with this body.
		const class HitMask& HitMask() const;
		// Sets the hit mask associated with this body.
		void SetHitMask(const class HitMask& mask);

		ColliderFlags Flags() const;
		void SetFlags(ColliderFlags flags);

		void OnCollision(CollisionEventArgs& e);

	private:
		ICollisionListener* _listener;
		class HitMask _hitmask;
		CollisionFilter _filter;
		ColliderFlags _flags;

	};

}