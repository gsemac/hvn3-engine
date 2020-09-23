#pragma once
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	struct CollisionResult;
	class ICollider;

	class INarrowPhase {

	public:
		virtual bool TestCollision(ICollider* body, ICollider* other, CollisionResult& manifold) const = 0;
		virtual bool TestCollision(ICollider* body, const PointF& body_position, ICollider* other, const PointF& other_position, CollisionResult& manifold) const = 0;

	};

}