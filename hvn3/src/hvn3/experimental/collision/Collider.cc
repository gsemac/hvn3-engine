#include "Collider.h"
#include "RoomBase.h"
#include "Object.h"

namespace hvn3 {

	Collider::Collider(Positionable2dBase<float>* tracking_object) :
		_tracking_object(tracking_object) {

		_mask = nullptr;

	}

	RectangleF Collider::AABB() const {

		float x = _tracking_object->X();
		float y = _tracking_object->Y();

		// Return an empty rectangle if the collider doesn't have a mask.
		if (_mask == nullptr)
			return RectangleF(x, y, 0, 0);

		RectangleF aabb = _mask->AABB();

		aabb.Translate(x, y);

		return aabb;

	}

	HitMaskPtr& Collider::HitMask() {
		
		return _mask;

	}
	const HitMaskPtr& Collider::HitMask() const {

		return _mask;

	}
	void Collider::SetHitMask(HitMaskPtr& mask) {

		_mask = std::move(mask);

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