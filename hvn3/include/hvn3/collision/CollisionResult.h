#pragma once
#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class ICollider;
	class IObject;

	struct CollisionResult {

		CollisionResult();

		ICollider* collider;
		IObject* object;

		PointF position;
		Vector2d normal;
		float penetrationDepth;

	};

}