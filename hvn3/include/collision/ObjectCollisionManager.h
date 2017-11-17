#pragma once
#include "collision/CollisionManagerBase.h"
#include "collision/ObjectCollisionBody.h"
#include <unordered_map>

namespace hvn3 {

	template <typename broadphase_type, typename narrowphase_type>
	class ObjectCollisionManager final : public CollisionManagerBase<broadphase_type, narrowphase_type, ObjectCollisionBody> {

	public:
		broadphase_type& BroadPhase() override {

			return _broad_phase;

		}
		narrowphase_type& NarrowPhase() override {

			return _narrow_phase;

		}

		const std::vector<CollisionManifold>& CollidingPairs() const override {

			return _pairs;

		}

		void Clear() override {

			_broad_phase.Clear();

			_bodies.clear();

		}

		void OnUpdate(UpdateEventArgs& e) override {

			// Update the state of the collision detection method.
			_broad_phase.OnUpdate(e);

			// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
			_checkPairs(_broad_phase.FindCandidatePairs());

		}

	protected:
		CollisionBodyHandle AddBody(body_type&& body) override {

			_bodies.emplace_back(std::move(body));

			body_type* ptr = &_bodies.back();
			_broad_phase.AddBody(ptr);

			return CollisionBodyHandle(ptr);

		}

	private:
		broadphase_type _broad_phase;
		narrowphase_type _narrow_phase;
		std::vector<CollisionManifold> _pairs;
		std::list<body_type> _bodies;

		void _checkPairs(const IBroadPhase::collider_pair_vector_type& pairs) {

			// Clear the last update's list of colliding pairs.
			_pairs.clear();

			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				ObjectCollisionBody* body_1 = static_cast<ObjectCollisionBody*>(i->first);
				ObjectCollisionBody* body_2 = static_cast<ObjectCollisionBody*>(i->second);

				CollisionManifold m;
				m.BodyA = body_1;
				m.BodyB = body_2;

				if (!_narrow_phase.TestCollision(body_1, body_2, m))
					continue;

				// Call the collision event for the first object.
				if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
					body_1->GetObject()->OnCollision(CollisionEventArgs(body_2->GetObject(), body_2));

				// Call the collision event for the second object.
				if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
					body_2->GetObject()->OnCollision(CollisionEventArgs(body_1->GetObject(), body_2));

				// Add the pair to the list of collisions.
				_pairs.push_back(m);

			}

		}

	};

}