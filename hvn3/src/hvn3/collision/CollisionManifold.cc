#include "hvn3/collision/CollisionManifold.h"

namespace hvn3 {

	CollisionManifold::CollisionManifold() :
		bodyA(nullptr),
		bodyB(nullptr),
		normal(0.0f, 0.0f),
		penetrationDepth(0.0f) {
	}

}