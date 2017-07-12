#include "RectangleHitMask.h"
#include "Exception.h"
#include "Geometry.h"

namespace hvn3 {

	RectangleHitMask::RectangleHitMask(const RectangleF& bounds) :
		_mask(bounds) {}

	void RectangleHitMask::SetOffset(const PointF& offset) {

		_mask.SetPosition(offset);
		
	}
	PointF RectangleHitMask::Offset() const {

		return _mask.Position();

	}
	RectangleF RectangleHitMask::AABB() const {

		return _mask;

	}

	bool RectangleHitMask::TestCollision(const HitMaskPtr& other, CollisionManifold& manifold) const {

		return other->TestCollision(_mask, manifold);

	}
	bool RectangleHitMask::TestCollision(const RectangleF& other, CollisionManifold& manifold) const {

		return hvn3::TestIntersection(_mask, other);

	}
	bool RectangleHitMask::TestCollision(const CircleF& other, CollisionManifold& manifold) const {

		return hvn3::TestIntersection(_mask, other);

	}
	bool RectangleHitMask::TestCollision(const LineF& other, CollisionManifold& manifold) const {

		return hvn3::TestIntersection(_mask, other);

	}

}