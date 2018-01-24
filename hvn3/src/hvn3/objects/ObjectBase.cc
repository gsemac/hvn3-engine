#include "hvn3/objects/ObjectBase.h"

namespace hvn3 {

	ObjectBase::ObjectBase() {

		_is_destroyed = false;
		_is_active = true;
		_flags = static_cast<ObjectFlags>(0);

	}
	void ObjectBase::OnCreate(CreateEventArgs& e) {}
	void ObjectBase::OnDestroy(DestroyEventArgs& e) {}
	void ObjectBase::OnDraw(DrawEventArgs& e) {}
	void ObjectBase::OnUpdate(UpdateEventArgs& e) {}
	void ObjectBase::Destroy() {

		_is_destroyed = true;

	}
	bool ObjectBase::IsDestroyed() const {

		return _is_destroyed;

	}
	void ObjectBase::Activate() {

		_is_active = true;

	}
	void ObjectBase::Deactivate() {

		_is_active = false;

	}
	bool ObjectBase::IsActive() const {

		return _is_active;

	}
	ObjectFlags ObjectBase::Flags() const {

		return _flags;

	}
	void ObjectBase::SetFlags(ObjectFlags flags) {

		_flags = flags;

	}

}