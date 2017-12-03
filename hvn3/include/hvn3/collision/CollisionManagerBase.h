#pragma once
#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/CollisionBodyMutator.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/collision/IBroadPhase.h"
#include <vector>

namespace hvn3 {

	class ICollisionBody;

	template <typename broadphase_type, typename narrowphase_type, typename body_type>
	class CollisionManagerBase : public ICollisionManager {

	public:
		typedef broadphase_type broadphase_type;
		typedef narrowphase_type narrowphase_type;
		typedef body_type body_type;

		CollisionManagerBase() = default;
		~CollisionManagerBase() {

			// Clear all bodies from the broad phase manager, which will clear each one's manager pointer.
			_broad_phase.Clear();

			//// Remove self as the manager for all collision bodies.
			//// This is so that, when the bodies fall out of scope, they do not try to remove themselves from the manager.
			//for (auto i = Bodies().begin(); i != Bodies().end(); ++i)
			//	System::CollisionBodyMutator(*i).SetManager(nullptr);

		}

		void AddBody(ICollisionBody& body) override {

			System::CollisionBodyMutator(body).SetManager(this);

			_broad_phase.AddBody(body);

		}
		void RemoveBody(ICollisionBody& body) override {

			System::CollisionBodyMutator(body).SetManager(nullptr);

			_broad_phase.RemoveBody(body);

		}
		broadphase_type& BroadPhase() override {

			return _broad_phase;

		}
		narrowphase_type& NarrowPhase() override {

			return _narrow_phase;

		}
		const std::vector<CollisionManifold>& CollidingPairs() const override {

			return _pairs;

		}
		size_t Count() const override {

			return _broad_phase.Count();

		}
		void Clear() override {

			_broad_phase.Clear();

		}

		void OnUpdate(UpdateEventArgs& e) override {

			// Update the state of the collision detection method.
			_broad_phase.OnUpdate(e);

			// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
			CheckPairs(_broad_phase.FindCandidatePairs());

		}

	protected:
		// Returns the vector of colliding pairs from the last update.
		std::vector<CollisionManifold>& Pairs() {

			return _pairs;

		}
		// Returns the vector of colliding pairs from the last update.
		const std::vector<CollisionManifold>& Pairs() const {

			return _pairs;

		}
		// Checks all potentially-colliding pairs and triggers events as needed.
		virtual void CheckPairs(const typename broadphase_type::collider_pair_vector_type& pairs) = 0;

	private:
		std::vector<CollisionManifold> _pairs;
		broadphase_type _broad_phase;
		narrowphase_type _narrow_phase;

	};

}