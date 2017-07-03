#include "ObjectBase.h"

namespace hvn3 {

	ObjectBase::ObjectBase() {

		_is_destroyed = false;
		_active = false;

	}
	void ObjectBase::OnCreate(CreateEventArgs& e) {}
	void ObjectBase::OnDraw(DrawEventArgs& e) {}
	void ObjectBase::OnUpdate(UpdateEventArgs& e) {}
	void ObjectBase::Destroy() {

		_is_destroyed = true;

	}
	bool ObjectBase::IsDestroyed() {

		return _is_destroyed;

	}
	void ObjectBase::Deactivate() {

		_active = false;

	}
	bool ObjectBase::IsActive() {

		return _active;

	}

}