#pragma once
#include "collision/CollisionManagerBase.h"
#include "collision/ObjectCollisionBody.h"
#include <unordered_map>

namespace hvn3 {

	template <typename broadphase_type, typename narrowphase_type>
	class ObjectCollisionManager final : public CollisionManagerBase<broadphase_type, narrowphase_type, ObjectCollisionBody> {

	protected:
		void CheckPairs(const typename broadphase_type::collider_pair_vector_type& pairs) override {

			// Clear the last update's list of colliding pairs.
			Pairs().clear();

			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				ObjectCollisionBody* body_1 = static_cast<ObjectCollisionBody*>(i->first);
				ObjectCollisionBody* body_2 = static_cast<ObjectCollisionBody*>(i->second);

				CollisionManifold m;
				m.BodyA = body_1;
				m.BodyB = body_2;

				if (!NarrowPhase().TestCollision(body_1, body_2, m))
					continue;

				// Call the collision event for the first object.
				if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
					body_1->GetObject()->OnCollision(CollisionEventArgs(body_2->GetObject(), body_2));

				// Call the collision event for the second object.
				if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
					body_2->GetObject()->OnCollision(CollisionEventArgs(body_1->GetObject(), body_2));

				// Add the pair to the list of collisions.
				Pairs().push_back(m);

			}

		}

	};

}