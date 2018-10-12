#pragma once
#include "hvn3/collision/CollisionManagerBase.h"
#include "hvn3/collision/Collider.h"

namespace hvn3 {

	class CollisionManager :
		public CollisionManagerBase {

	protected:
		using CollisionManagerBase::CollisionManagerBase;

		void CheckPairs(const IBroadPhase::collider_pair_vector_type& pairs) override {

			// Clear the last update's list of colliding pairs.
			this->GetPairs().clear();

			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				ICollider* body_1 = i->first;
				ICollider* body_2 = i->second;

				CollisionResult m;

				if (!Narrow().TestCollision(body_1, body_2, m))
					continue;

				// Call the collision event for the first object.
				if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
					body_1->OnCollide(body_2);

				// Call the collision event for the second object.
				if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
					body_2->OnCollide(body_1);

				// Add the pair to the list of collisions.
				this->GetPairs().push_back(m);

			}

		}

	};

}