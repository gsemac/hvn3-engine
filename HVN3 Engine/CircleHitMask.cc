#include "CircleHitMask.h"
#include "Exception.h"
#include "Geometry.h"

namespace hvn3 {

	CircleHitMask::CircleHitMask(const CircleF& mask) :
		_mask(mask) {}
	void CircleHitMask::SetOffset(const PointF& offset) {

		_mask.SetPosition(offset);

	}
	PointF CircleHitMask::Offset() const {

		return _mask.Position();

	}
	RectangleF CircleHitMask::AABB() const {

		return RectangleF(
			PointF(_mask.X() - _mask.Radius(), _mask.Y() - _mask.Radius()), 
			PointF(_mask.X() + _mask.Radius(), _mask.Y() + _mask.Radius())
		);

	}

	bool CircleHitMask::TestIntersection(const IHitMask* other) const {

		return other->TestIntersection(_mask);

	}
	bool CircleHitMask::TestIntersection(const RectangleF& other) const {

		return hvn3::TestIntersection(other, _mask);

	}
	bool CircleHitMask::TestIntersection(const CircleF& other) const {

		return hvn3::TestIntersection(_mask, other);

	}
	bool CircleHitMask::TestIntersection(const LineF& other) const {

		return hvn3::TestIntersection(_mask, other);

	}

}