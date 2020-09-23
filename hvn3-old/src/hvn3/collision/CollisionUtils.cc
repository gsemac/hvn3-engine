#include "hvn3/collision/CollisionUtils.h"

namespace hvn3 {

	bool TestIntersection(HitMask& body1, HitMask& body2) {
		return static_cast<bool>(TestCollision(body1, body2));
	}
	bool TestIntersectionAt(HitMask& body1, const PointF& at1, HitMask& body2, const PointF& at2) {
		return static_cast<bool>(TestCollisionAt(body1, at1, body2, at2));
	}

	CollisionInfo TestCollision(HitMask& body1, HitMask& body2) {

		switch (body1.Type()) {

		case HitMask::HITMASK_TYPE::HITMASK_TYPE_RECTANGLE:

			switch (body2.Type()) {

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_RECTANGLE:
				return TestIntersection(body1.GetBody<HitMask::rectangle_type>(), body2.GetBody<HitMask::rectangle_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_LINE:
				return TestIntersection(body1.GetBody<HitMask::rectangle_type>(), body2.GetBody<HitMask::line_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_CIRCLE:
				return TestIntersection(body1.GetBody<HitMask::rectangle_type>(), body2.GetBody<HitMask::circle_type>());

			}

		case HitMask::HITMASK_TYPE::HITMASK_TYPE_LINE:

			switch (body2.Type()) {

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_RECTANGLE:
				return TestIntersection(body1.GetBody<HitMask::line_type>(), body2.GetBody<HitMask::rectangle_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_LINE:
				return TestIntersection(body1.GetBody<HitMask::line_type>(), body2.GetBody<HitMask::line_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_CIRCLE:
				return TestIntersection(body1.GetBody<HitMask::line_type>(), body2.GetBody<HitMask::circle_type>());

			}

		case HitMask::HITMASK_TYPE::HITMASK_TYPE_CIRCLE:

			switch (body2.Type()) {

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_RECTANGLE:
				return TestIntersection(body1.GetBody<HitMask::circle_type>(), body2.GetBody<HitMask::rectangle_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_LINE:
				return TestIntersection(body1.GetBody<HitMask::circle_type>(), body2.GetBody<HitMask::line_type>());

			case HitMask::HITMASK_TYPE::HITMASK_TYPE_CIRCLE:
				return TestCollision(body1.GetBody<HitMask::circle_type>(), body2.GetBody<HitMask::circle_type>());

			}

		}

		// This point should never be reached.
		return CollisionInfo(false);

	}
	CollisionInfo TestCollisionAt(HitMask& body1, const PointF& at1, HitMask& body2, const PointF& at2) {

		PointF offset_1 = body1.Offset();
		PointF offset_2 = body2.Offset();

		CollisionInfo result = TestCollision(body1, body2);

		body1.SetOffset(offset_1);
		body2.SetOffset(offset_2);

		return result;

	}

}