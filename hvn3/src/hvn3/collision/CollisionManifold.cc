#include "hvn3/collision/CollisionManifold.h"

namespace hvn3 {

	CollisionManifold::CollisionManifold() :
		BodyA(nullptr),
		BodyB(nullptr),
		Normal(0.0f, 0.0f),
		FaceNormal(0.0f, 0.0f),
		PenetrationDepth(0.0f) {
	}

}