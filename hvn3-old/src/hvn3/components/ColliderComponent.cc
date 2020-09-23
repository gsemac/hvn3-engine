#include "hvn3/components/ColliderComponent.h"

namespace hvn3 {

	constexpr ColliderFlags DEFAULT_COLLISION_BODY_FLAGS = static_cast<ColliderFlags>(0);

	// Public methods

	ColliderComponent::ColliderComponent() :
		_flags(DEFAULT_COLLISION_BODY_FLAGS) {
	}

	RectangleF ColliderComponent::Aabb() const {
		return _hitmask.Aabb();
	}

	CollisionFilter& ColliderComponent::GetFilter() {
		return _filter;
	}
	const CollisionFilter& ColliderComponent::Filter() const {
		return _filter;
	}

	HitMask& ColliderComponent::GetHitMask() {
		return _hitmask;
	}
	const class HitMask& ColliderComponent::HitMask() const {
		return _hitmask;
	}
	void ColliderComponent::SetHitMask(const class HitMask& mask) {
		_hitmask = mask;
	}

	ColliderFlags ColliderComponent::Flags() const {
		return _flags;
	}
	void ColliderComponent::SetFlags(ColliderFlags flags) {
		_flags = flags;
	}

	void ColliderComponent::OnCollision(CollisionEventArgs& e) {
	
	}

}