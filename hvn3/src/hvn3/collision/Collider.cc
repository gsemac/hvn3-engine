#include "hvn3/collision/Collider.h"
#include "hvn3/objects/IObject.h"

namespace hvn3 {

	Collider::Collider(IObject* object) {

		_object = object;

	}
	Collider::Collider(Collider&& other) {

		_object = other._object;

		other._object = nullptr;

	}
	float Collider::X() const {

		return _object->X();

	}
	float Collider::Y() const {

		return _object->Y();

	}
	PointF Collider::Position() const {

		return _object->Position();

	}
	void Collider::SetX(float x) {

		_object->SetX(x);

	}
	void Collider::SetY(float y) {

		_object->SetY(y);

	}
	void Collider::SetPosition(const PointF& position) {

		_object->SetPosition(position);

	}
	void Collider::OnCollide(ICollider* other) const {

		_object->OnCollide(CollisionEventArgs(other));

	}

}