#include "ObjectCollisionBody.h"

namespace hvn3 {

	ObjectCollisionBody::ObjectCollisionBody(ObjectPtr& object) {

		_key = object.get();
		_object = object;
		_solid = false;

	}

	float ObjectCollisionBody::X() const {

		return GetObject()->X();

	}
	float ObjectCollisionBody::Y() const {

		return GetObject()->Y();

	}
	void ObjectCollisionBody::SetX(float x) {

		GetObject()->SetX(x);

	}
	void ObjectCollisionBody::SetY(float y) {

		GetObject()->SetY(y);

	}
	PointF ObjectCollisionBody::Position() const {

		return GetObject()->Position();

	}
	void ObjectCollisionBody::SetPosition(const PointF& position) {

		GetObject()->SetPosition(position);

	}
	void ObjectCollisionBody::SetPosition(float x, float y) {

		GetObject()->SetPosition(x, y);

	}
	RectangleF ObjectCollisionBody::AABB() const {

		float x = GetObject()->X();
		float y = GetObject()->Y();

		// Return an empty rectangle if the collider doesn't have a mask.
		if (!HitMask())
			return RectangleF(GetObject()->X(), GetObject()->Y(), 0, 0);

		RectangleF aabb = HitMask()->AABB();
		aabb.Translate(GetObject()->X(), GetObject()->Y());

		return aabb;

	}
	CollisionFilter& ObjectCollisionBody::Filter() {

		return _filter;

	}
	const CollisionFilter& ObjectCollisionBody::Filter() const {

		return _filter;

	}
	HitMaskPtr& ObjectCollisionBody::HitMask() {

		return _mask;

	}
	const HitMaskPtr& ObjectCollisionBody::HitMask() const {

		return _mask;

	}
	void ObjectCollisionBody::SetHitMask(HitMaskPtr& mask) {

		_mask = std::move(mask);

	}
	bool ObjectCollisionBody::IsSolid() const {

		return _solid;

	}
	void ObjectCollisionBody::SetSolid(bool value) {
	
		_solid = value;

	}

	Object* ObjectCollisionBody::GetObject() {

		return _key;

	}
	const Object* ObjectCollisionBody::GetObject() const {

		return _key;

	}
	bool ObjectCollisionBody::ObjectExpired() const {

		return _object.expired();

	}

}