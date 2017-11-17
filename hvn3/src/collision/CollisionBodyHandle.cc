#include "collision/CollisionBodyHandle.h"
#include "collision/ICollisionBody.h"

namespace hvn3 {

	CollisionBodyHandle::CollisionBodyHandle() {

		_manager = nullptr;
		_body = nullptr;

	}
	CollisionBodyHandle::CollisionBodyHandle(ICollisionBody* body) {

		_body = body;

	}
	CollisionBodyHandle::~CollisionBodyHandle() {

		if (_body != nullptr)
			_body->Destroy();

	}

	CollisionBodyHandle::CollisionBodyHandle(CollisionBodyHandle&& other) {

		*this = std::move(other);

	}

	ICollisionBody* CollisionBodyHandle::operator->() const {

		return _body;

	}
	CollisionBodyHandle& CollisionBodyHandle::operator=(CollisionBodyHandle&& other) {

		_body = other._body;

		other._body = nullptr;

		return *this;

	}

}