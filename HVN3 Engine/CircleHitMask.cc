#include "CircleHitMask.h"
#include "Exception.h"
#include "Geometry.h"

namespace hvn3 {
	namespace Collision {

		CircleHitMask::CircleHitMask(const CircleF& mask) :
			_mask(mask) {
		}
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

		bool CircleHitMask::TestCollision(const HitMaskPtr& other, CollisionManifold& manifold) const {

			return other->TestCollision(_mask, manifold);

		}
		bool CircleHitMask::TestCollision(const RectangleF& other, CollisionManifold& manifold) const {

			return Math::Geometry::TestIntersection(other, _mask);

		}
		bool CircleHitMask::TestCollision(const CircleF& other, CollisionManifold& manifold) const {

			return Math::Geometry::TestIntersection(_mask, other);

		}
		bool CircleHitMask::TestCollision(const LineF& other, CollisionManifold& manifold) const {

			return Math::Geometry::TestIntersection(_mask, other);

		}

	}
}