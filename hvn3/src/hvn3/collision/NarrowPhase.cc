#include "hvn3/collision/NarrowPhase.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/math/GeometryUtils.h"

namespace hvn3 {

	bool NarrowPhase::TestCollision(ICollisionBody* a, ICollisionBody* b, CollisionManifold& m) const {

		return TestCollision(a, a->Position(), b, b->Position(), m);

	}
	bool NarrowPhase::TestCollision(ICollisionBody* a, const PointF& position_a, ICollisionBody* b, const PointF& position_b, CollisionManifold& m) const {

		// Get the masks for both colliders.
		HitMaskPtr& a_mask = a->HitMask();
		HitMaskPtr& b_mask = b->HitMask();
		bool hit;

		// Offset both colliders by the given offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() + position_a.X(), a_mask->Offset().Y() + position_a.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() + position_b.X(), b_mask->Offset().Y() + position_b.Y()));

		// If the bounding boxes do not intersect, return false.
		if (!Math::Geometry::TestIntersection(a_mask->AABB(), b_mask->AABB()))
			hit = false;

		// Test to see if the two masks intersect one another.
		else
			hit = a_mask->TestCollision(b_mask, m);

		// Reset the offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() - position_a.X(), a_mask->Offset().Y() - position_a.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() - position_b.X(), b_mask->Offset().Y() - position_b.Y()));

		// Return the result.
		return hit;

	}

}