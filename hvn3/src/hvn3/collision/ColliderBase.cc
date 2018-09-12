#include "hvn3/collision/ColliderBase.h"
#define HVN3_DEFAULT_COLLISION_BODY_FLAGS static_cast<hvn3::ColliderFlags>(0)

namespace hvn3 {

	ColliderBase::ColliderBase() :
		_flags(HVN3_DEFAULT_COLLISION_BODY_FLAGS) {
		_is_destroyed = false;
	}

	PointF ColliderBase::Position() const {

		return PointF(X(), Y());

	}
	void ColliderBase::SetPosition(const PointF& position) {

		SetPosition(position.X(), position.Y());

	}
	void ColliderBase::SetPosition(float x, float y) {

		SetX(x);
		SetY(y);

	}
	RectangleF ColliderBase::AABB() const {

		// A rectangle of area 0 is returned if the body doesn't have a hitmask.
		if (!HitMask())
			return RectangleF(X(), Y(), 0, 0);

		RectangleF aabb = HitMask()->AABB();
		aabb.Translate(X(), Y());

		return aabb;

	}
	CategoryFilter& ColliderBase::GetCategory() {

		return _filter;

	}
	const CategoryFilter& ColliderBase::Category() const {

		return _filter;

	}
	HitMaskPtr& ColliderBase::GetHitMask() {

		return _mask;

	}
	const HitMaskPtr& ColliderBase::HitMask() const {

		return _mask;

	}
	void ColliderBase::SetHitMask(HitMaskPtr& mask) {

		_mask = std::move(mask);

	}

	ColliderFlags ColliderBase::Flags() const {

		return _flags;

	}
	void ColliderBase::SetFlags(ColliderFlags flags) {
		_flags = flags;
	}

	void ColliderBase::Destroy() {
		_is_destroyed = true;
	}
	bool ColliderBase::IsDestroyed() const {
		return _is_destroyed;
	}
	void ColliderBase::OnDestroy(DestroyEventArgs& e) {}

}