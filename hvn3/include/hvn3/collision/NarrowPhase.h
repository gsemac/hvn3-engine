#pragma once
#include "hvn3/collision/INarrowPhase.h"

namespace hvn3 {

	class NarrowPhase : public INarrowPhase {

	public:
		bool TestCollision(ICollisionBody* a, ICollisionBody* b, CollisionManifold& m) const override;
		bool TestCollision(ICollisionBody* a, const PointF& position_a, ICollisionBody* b, const PointF& position_b, CollisionManifold& m) const override;

	};

}