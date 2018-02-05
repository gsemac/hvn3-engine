#pragma once
#include "hvn3/collision/INarrowPhase.h"

namespace hvn3 {

	class NarrowPhase : public INarrowPhase {

	public:
		bool TestCollision(ICollisionBody* body, ICollisionBody* other, CollisionManifold& manifold) const override;
		bool TestCollision(ICollisionBody* body, const PointF& body_position, ICollisionBody* other, const PointF& other_position, CollisionManifold& manifold) const override;

	};

}