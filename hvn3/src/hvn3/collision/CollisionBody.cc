#include "hvn3/collision/CollisionBody.h"
#include "hvn3/objects/IObject.h"

namespace hvn3 {

	CollisionBody::CollisionBody(IObject* object) {

		_object = object;

	}
	CollisionBody::CollisionBody(CollisionBody&& other) {

		_object = other._object;

		other._object = nullptr;

	}

	float CollisionBody::X() const {

		return _object->X();

	}
	float CollisionBody::Y() const {

		return _object->Y();

	}
	PointF CollisionBody::Position() const {

		return _object->Position();

	}
	void CollisionBody::SetX(float x) {

		_object->SetX(x);

	}
	void CollisionBody::SetY(float y) {

		_object->SetY(y);

	}
	void CollisionBody::SetPosition(const PointF& position) {

		_object->SetPosition(position);

	}

	IObject* CollisionBody::GetObject() {
		return _object;
	}

}