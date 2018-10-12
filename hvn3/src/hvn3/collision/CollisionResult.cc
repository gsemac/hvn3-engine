#include "hvn3/collision/CollisionResult.h"

namespace hvn3 {

	CollisionResult::CollisionResult() :
		collider(nullptr),
		object(nullptr),
		position(0.0f, 0.0f),
		normal(0.0f, 0.0f),
		penetrationDepth(0.0f) {
	}

}