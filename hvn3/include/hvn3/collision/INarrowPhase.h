#pragma once
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	struct CollisionManifold;
	class ICollisionBody;

	class INarrowPhase {

	public:
		virtual bool TestCollision(ICollisionBody* body, ICollisionBody* other, CollisionManifold& manifold) const = 0;
		virtual bool TestCollision(ICollisionBody* body, const PointF& body_position, ICollisionBody* other, const PointF& other_position, CollisionManifold& manifold) const = 0;

	};

}