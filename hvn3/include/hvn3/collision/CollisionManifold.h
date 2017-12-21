#pragma once
#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class ICollisionBody;

	struct CollisionManifold {

		CollisionManifold();

		ICollisionBody* BodyA;
		ICollisionBody* BodyB;
		Vector2d Normal;
		Vector2d FaceNormal;
		float PenetrationDepth;

	};

}