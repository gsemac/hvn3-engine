#pragma once

namespace hvn3 {

	class ICollisionBody;

	class CollisionBodyPtr {

	public:
		CollisionBodyPtr();
		CollisionBodyPtr(ICollisionBody* ptr);

		ICollisionBody* get();
		const ICollisionBody* get() const;

		ICollisionBody* operator->();
		const ICollisionBody* operator->() const;

		explicit operator bool() const;

		bool operator==(const ICollisionBody* rhs) const;

	private:
		ICollisionBody* _ptr;

	};

}