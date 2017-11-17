#include "collision/ObjectCollisionBody.h"

namespace hvn3 {

	ObjectCollisionBody::ObjectCollisionBody(ObjectPtr& object) {

		_key = object.get();
		_object = object;

	}
	ObjectCollisionBody::ObjectCollisionBody(ObjectCollisionBody&& other) {

		_key = other._key;
		_object = std::move(other._object);

		other._key = nullptr;

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

		return _key;

	}
	const Object* ObjectCollisionBody::GetObject() const {

		return _key;

	}
	bool ObjectCollisionBody::ObjectExpired() const {

		return _object.expired();

	}

	bool ObjectCollisionBody::IsDestroyed() const {

		return CollisionBodyBase::IsDestroyed() || ObjectExpired();

	}

}