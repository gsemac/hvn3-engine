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

		RectangleF aabb = HitMask().AABB();
		aabb.Translate(X(), Y());

		return aabb;

	}
	CollisionFilter& ColliderBase::GetCategory() {

		return _filter;

	}
	const CollisionFilter& ColliderBase::Category() const {

		return _filter;

	}
	HitMask& ColliderBase::GetHitMask() {

		return _mask;

	}
	const HitMask& ColliderBase::HitMask() const {

		return _mask;

	}
	void ColliderBase::SetHitMask(const hvn3::HitMask& mask) {
		_mask = mask;
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