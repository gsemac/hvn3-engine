#include "ObjectCollisionBody.h"
#include "Object.h"

namespace hvn3 {

	ObjectCollisionBody::ObjectCollisionBody(Object* object) {

		_object = object;
		_solid = false;

	}

	float ObjectCollisionBody::X() const {

		return _object->X();

	}
	float ObjectCollisionBody::Y() const {

		return _object->Y();

	}
	void ObjectCollisionBody::SetX(float x) {

		_object->SetX(x);

	}
	void ObjectCollisionBody::SetY(float y) {

		_object->SetY(y);

	}
	PointF ObjectCollisionBody::Position() const {

		return _object->Position();

	}
	void ObjectCollisionBody::SetPosition(const PointF& position) {

		_object->SetPosition(position);

	}
	void ObjectCollisionBody::SetPosition(float x, float y) {

		_object->SetPosition(x, y);

	}
	RectangleF ObjectCollisionBody::AABB() const {

		float x = _object->X();
		float y = _object->Y();

		// Return an empty rectangle if the collider doesn't have a mask.
		if (!HitMask())
			return RectangleF(_object->X(), _object->Y(), 0, 0);

		RectangleF aabb = HitMask()->AABB();
		aabb.Translate(_object->X(), _object->Y());

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

		return _object;

	}

}