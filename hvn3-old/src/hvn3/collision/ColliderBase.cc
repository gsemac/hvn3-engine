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

		RectangleF aabb = HitMask().Aabb().Translate(X(), Y());
		
		return aabb;

	}
	CollisionFilter& ColliderBase::Filter() {
		return _filter;
	}
	const CollisionFilter& ColliderBase::Filter() const {
		return _filter;
	}
	HitMask& ColliderBase::HitMask() {
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

}