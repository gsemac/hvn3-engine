#pragma once
#include "Vector2d.h"

namespace hvn3 {
	namespace Collision {

		class ICollisionBody;

		struct CollisionManifold {

			CollisionManifold();

			ICollisionBody* BodyA;
			ICollisionBody* BodyB;
			Vector2d Normal;
			float Penetration;
		
		};

	}
}