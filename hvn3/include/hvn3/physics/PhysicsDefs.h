#pragma once

#include <memory>

namespace hvn3 {

	namespace Physics {
		class IPhysicsBody;
		class PhysicsBodyBase;
	}

	typedef std::shared_ptr<Physics::IPhysicsBody> IPhysicsBodyPtr;
	typedef IPhysicsBodyPtr PhysicsBodyPtr;

	template<typename PhysicsBodyType = Physics::PhysicsBodyBase, typename... Args>
	PhysicsBodyPtr make_physics_body(Args&&... args) {

		PhysicsBodyPtr ptr = std::make_shared<PhysicsBodyType>(std::forward<Args>(args)...);

		return ptr;

	}

}