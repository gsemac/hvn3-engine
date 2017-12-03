#include "hvn3/collision/CollisionManifold.h"

namespace hvn3 {

	CollisionManifold::CollisionManifold() :
		BodyA(nullptr),
		BodyB(nullptr),
		Normal(0.0f, 0.0f),
		Penetration(0.0f) {
	}

}