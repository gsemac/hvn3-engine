#pragma once
#include "IHitMask.h"

namespace hvn3 {
	namespace Collision {

	struct CollisionManifold;

	class CircleHitMask : public IHitMask {

	public:
		CircleHitMask(const CircleF& mask);

		void SetOffset(const PointF& offet) override;
		PointF Offset() const override;

		RectangleF AABB() const override;

		bool TestCollision(const HitMaskPtr& other, CollisionManifold& manifold) const override;
		bool TestCollision(const RectangleF& other, CollisionManifold& manifold) const override;
		bool TestCollision(const CircleF& other, CollisionManifold& manifold) const override;
		bool TestCollision(const LineF& other, CollisionManifold& manifold) const override;

	private:
		CircleF _mask;

	};

	}
}