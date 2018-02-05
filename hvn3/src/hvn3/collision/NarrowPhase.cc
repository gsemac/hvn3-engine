#include "hvn3/collision/NarrowPhase.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/math/GeometryUtils.h"

namespace hvn3 {

	bool NarrowPhase::TestCollision(ICollisionBody* body, ICollisionBody* other, CollisionManifold& manifold) const {

		return TestCollision(body, body->Position(), other, other->Position(), manifold);

	}
	bool NarrowPhase::TestCollision(ICollisionBody* body, const PointF& body_position, ICollisionBody* other, const PointF& other_position, CollisionManifold& manifold) const {

		// Get the masks for both colliders.
		HitMaskPtr& a_mask = body->HitMask();
		HitMaskPtr& b_mask = other->HitMask();
		bool hit;

		// Offset both colliders by the given offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() + body_position.X(), a_mask->Offset().Y() + body_position.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() + other_position.X(), b_mask->Offset().Y() + other_position.Y()));

		// If the bounding boxes do not intersect, return false.
		if (!Math::Geometry::TestIntersection(a_mask->AABB(), b_mask->AABB()))
			hit = false;

		// Test to see if the two masks intersect one another.
		else
			hit = a_mask->TestCollision(b_mask, manifold);

		// Reset the offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() - body_position.X(), a_mask->Offset().Y() - body_position.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() - other_position.X(), b_mask->Offset().Y() - other_position.Y()));

		// Return the result.
		return hit;

	}

}