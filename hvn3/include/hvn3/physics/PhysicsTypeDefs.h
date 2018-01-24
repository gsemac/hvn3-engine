#pragma once
#include <memory>

namespace hvn3 {

	class IPhysicsBody;

	typedef std::shared_ptr<IPhysicsBody> PhysicsBodyPtr;

}