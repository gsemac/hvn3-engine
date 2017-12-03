#pragma once
#include "hvn3/utility/Point2d.h"

namespace hvn3 {

	struct CollisionManifold;
	class ICollisionBody;

	class INarrowPhase {

	public:
		virtual bool TestCollision(ICollisionBody* a, ICollisionBody* b, CollisionManifold& m) const = 0;
		virtual bool TestCollision(ICollisionBody* a, const PointF& position_a, ICollisionBody* b, const PointF& position_b, CollisionManifold& m) const = 0;

	};

}