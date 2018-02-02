#pragma once
#include <memory>

namespace hvn3 {

	namespace Physics {
		class IPhysicsBody;
	}

	typedef std::shared_ptr<Physics::IPhysicsBody> PhysicsBodyPtr;

}