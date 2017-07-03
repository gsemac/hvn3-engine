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

	bool RectangleHitMask::TestIntersection(const HitMaskPtr& other) const {

		return other->TestIntersection(_mask);

	}
	bool RectangleHitMask::TestIntersection(const RectangleF& other) const {

		return hvn3::TestIntersection(_mask, other);

	}
	bool RectangleHitMask::TestIntersection(const CircleF& other) const {

		return hvn3::TestIntersection(_mask, other);

	}
	bool RectangleHitMask::TestIntersection(const LineF& other) const {

		return hvn3::TestIntersection(_mask, other);

	}

}