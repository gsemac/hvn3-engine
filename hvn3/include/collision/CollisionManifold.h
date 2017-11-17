#pragma once
#include "math/Vector2d.h"

namespace hvn3 {

	class ICollisionBody;

	struct CollisionManifold {

		CollisionManifold();

		ICollisionBody* BodyA;
		ICollisionBody* BodyB;
		Vector2d Normal;
		float Penetration;

	};

}