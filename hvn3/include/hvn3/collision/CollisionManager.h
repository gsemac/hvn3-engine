#pragma once
#include "hvn3/collision/CollisionManagerBase.h"
#include "hvn3/collision/CollisionBody.h"

namespace hvn3 {

	template <typename broadphase_type, typename narrowphase_type>
	class ObjectCollisionManager final : public CollisionManagerBase<IObject, CollisionBody, broadphase_type, narrowphase_type> {

	protected:
		void CheckPairs(const typename broadphase_type::collider_pair_vector_type& pairs) override {

			// Clear the last update's list of colliding pairs.
			this->GetPairs().clear();

			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				collider_ptr_type body_1 = static_cast<collider_ptr_type>(i->first);
				collider_ptr_type body_2 = static_cast<collider_ptr_type>(i->second);

				CollisionManifold m;
				m.bodyA = body_1;
				m.bodyB = body_2;

				if (!this->NarrowPhase().TestCollision(body_1, body_2, m))
					continue;

				// Call the collision event for the first object.
				if (body_1->Category().MaskBits() & body_2->Category().CategoryBits())
					body_1->GetObject()->OnCollide(CollisionEventArgs(body_2->GetObject(), body_2));

				// Call the collision event for the second object.
				if (body_2->Category().MaskBits() & body_1->Category().CategoryBits())
					body_2->GetObject()->OnCollide(CollisionEventArgs(body_1->GetObject(), body_2));

				// Add the pair to the list of collisions.
				this->GetPairs().push_back(m);

			}

		}

	};

}