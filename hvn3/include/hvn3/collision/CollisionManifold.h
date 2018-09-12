#pragma once
#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class ICollider;

	struct CollisionManifold {

		CollisionManifold();

		ICollider* bodyA;
		ICollider* bodyB;
		Vector2d normal;
		float penetrationDepth;

	};

}