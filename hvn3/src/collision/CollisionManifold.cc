#include "collision/CollisionManifold.h"

namespace hvn3 {
	namespace Collision {

		CollisionManifold::CollisionManifold() :
			BodyA(nullptr),
			BodyB(nullptr),
			Normal(0.0f, 0.0f),
			Penetration(0.0f) {
		}

	}
}