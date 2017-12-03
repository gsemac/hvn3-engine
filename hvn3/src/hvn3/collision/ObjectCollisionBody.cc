#include "hvn3/collision/ObjectCollisionBody.h"
#include "hvn3/objects/Object.h"

namespace hvn3 {

	ObjectCollisionBody::ObjectCollisionBody(Object* object) {

		_object = object;

	}
	ObjectCollisionBody::ObjectCollisionBody(ObjectCollisionBody&& other) {

		_object = other._object;

		other._object = nullptr;

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

	Object* ObjectCollisionBody::GetObject() {

		return _object;

	}
	const Object* ObjectCollisionBody::GetObject() const {

		return _object;

	}

}