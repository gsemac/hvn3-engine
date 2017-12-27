#pragma once
#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class ICollisionBody;

	struct CollisionManifold {

		CollisionManifold();

		ICollisionBody* bodyA;
		ICollisionBody* bodyB;
		Vector2d normal;
		float penetrationDepth;

	};

}