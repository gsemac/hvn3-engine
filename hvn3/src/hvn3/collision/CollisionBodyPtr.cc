#include "hvn3/collision/CollisionBodyPtr.h"

namespace hvn3 {

	CollisionBodyPtr::CollisionBodyPtr() {
		_ptr = nullptr;
	}
	CollisionBodyPtr::CollisionBodyPtr(ICollisionBody* ptr) :
		_ptr(ptr) {
	}

	ICollisionBody* CollisionBodyPtr::get() {
		return _ptr;
	}
	const ICollisionBody* CollisionBodyPtr::get() const {
		return _ptr;
	}

	ICollisionBody* CollisionBodyPtr::operator->() {
		return _ptr;
	}
	const ICollisionBody* CollisionBodyPtr::operator->() const {
		return _ptr;
	}

	CollisionBodyPtr::operator bool() const {
		return _ptr != nullptr;
	}

	bool CollisionBodyPtr::operator==(const ICollisionBody* rhs) const {
		return _ptr == rhs;
	}

}