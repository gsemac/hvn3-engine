#pragma once
#include "hvn3/collision/INarrowPhase.h"

namespace hvn3 {

	class NarrowPhase : 
		public INarrowPhase {

	public:
		bool TestCollision(ICollider* body, ICollider* other, CollisionResult& manifold) const override;
		bool TestCollision(ICollider* body, const PointF& body_position, ICollider* other, const PointF& other_position, CollisionResult& manifold) const override;

	};

}