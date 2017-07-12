#pragma once
#include "Vector2d.h"

namespace hvn3 {

	class ICollisionBody;

	struct CollisionManifold {
		ICollisionBody* BodyA;
		ICollisionBody* BodyB;
		Vector2d Normal;
		float Penetration;
	};

}