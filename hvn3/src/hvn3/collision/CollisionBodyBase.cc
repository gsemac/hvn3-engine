#include "hvn3/collision/CollisionBodyBase.h"
#define DEFAULT_COLLISION_BODY_FLAGS static_cast<hvn3::CollisionBodyFlags>(0)

namespace hvn3 {

	CollisionBodyBase::CollisionBodyBase() :
		_flags(DEFAULT_COLLISION_BODY_FLAGS) {
		_is_destroyed = false;
	}

	PointF CollisionBodyBase::Position() const {

		return PointF(X(), Y());

	}
	void CollisionBodyBase::SetPosition(const PointF& position) {

		SetPosition(position.X(), position.Y());

	}
	void CollisionBodyBase::SetPosition(float x, float y) {

		SetX(x);
		SetY(y);

	}
	RectangleF CollisionBodyBase::AABB() const {

		// A rectangle of area 0 is returned if the body doesn't have a hitmask.
		if (!HitMask())
			return RectangleF(X(), Y(), 0, 0);

		RectangleF aabb = HitMask()->AABB();
		aabb.Translate(X(), Y());

		return aabb;

	}
	CategoryFilter& CollisionBodyBase::GetCategory() {

		return _filter;

	}
	const CategoryFilter& CollisionBodyBase::Category() const {

		return _filter;

	}
	HitMaskPtr& CollisionBodyBase::GetHitMask() {

		return _mask;

	}
	const HitMaskPtr& CollisionBodyBase::HitMask() const {

		return _mask;

	}
	void CollisionBodyBase::SetHitMask(HitMaskPtr& mask) {

		_mask = std::move(mask);

	}

	CollisionBodyFlags CollisionBodyBase::Flags() const {

		return _flags;

	}
	void CollisionBodyBase::SetFlags(CollisionBodyFlags flags) {
		_flags = flags;
	}

	void CollisionBodyBase::Destroy() {
		_is_destroyed = true;
	}
	bool CollisionBodyBase::IsDestroyed() const {
		return _is_destroyed;
	}
	void CollisionBodyBase::OnDestroy(DestroyEventArgs& e) {}

}