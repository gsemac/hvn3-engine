#include "hvn3/collision/RectangleHitMask.h"
#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/GeometryUtils.h"	

namespace hvn3 {

	RectangleHitMask::RectangleHitMask(const RectangleF& bounds) :
		_mask(bounds) {
	}

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

		return ResolveCollision(_mask, other, manifold);

	}
	bool RectangleHitMask::TestCollision(const CircleF& other, CollisionManifold& manifold) const {

		return Math::Geometry::TestIntersection(_mask, other);

	}
	bool RectangleHitMask::TestCollision(const LineF& other, CollisionManifold& manifold) const {

		return Math::Geometry::TestIntersection(_mask, other);

	}

}