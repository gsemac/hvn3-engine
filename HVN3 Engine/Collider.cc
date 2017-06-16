#include "Collider.h"
#include "RoomBase.h"
#include "Object.h"

namespace hvn3 {

	Collider::Collider(Positionable2dBase<float>* tracking_object) :
		_tracking_object(tracking_object) {
	}

	RectangleF Collider::AABB() const {

		RectangleF aabb = _mask->AABB();

		aabb.Translate(_tracking_object->X(), _tracking_object->Y());

		return aabb;

	}

	IHitMask* Collider::HitMask() {

		return _mask;

	}
	const IHitMask* Collider::HitMask() const {

		return _mask;

	}
	void Collider::SetHitMask(IHitMask* mask) {

		_mask = mask;

		//_mask = std::move(mask);

	}

	CollisionFilter& Collider::Filter() {

		return _filter;

	}
	const CollisionFilter& Collider::Filter() const {

		return _filter;

	}

	Positionable2dBase<float>* Collider::TrackingObject() {

		return _tracking_object;

	}
	const Positionable2dBase<float>* Collider::TrackingObject() const {

		return _tracking_object;

	}

}