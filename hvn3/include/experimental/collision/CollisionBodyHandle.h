#pragma once

namespace hvn3 {

	class ICollisionManager;
	class ICollisionBody;

	class CollisionBodyHandle {

	public:
		CollisionBodyHandle();
		CollisionBodyHandle(ICollisionBody* body);
		~CollisionBodyHandle();

		CollisionBodyHandle(CollisionBodyHandle&& other);

		ICollisionBody* operator->() const;
		CollisionBodyHandle& operator=(CollisionBodyHandle&& other);

	private:
		ICollisionManager* _manager;
		ICollisionBody* _body;

	};

}