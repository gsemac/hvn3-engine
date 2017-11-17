#pragma once
#include "collision/CollisionManifold.h"
#include "collision/CollisionBodyHandle.h"
#include "collision/CollisionBodyMutator.h"
#include "collision/ICollisionManager.h"

namespace hvn3 {

	class ICollisionBody;

	template <typename broadphase_type, typename narrowphase_type, typename body_type>
	class CollisionManagerBase : public ICollisionManager {

	public:
		typedef broadphase_type broadphase_type;
		typedef narrowphase_type narrowphase_type;
		typedef body_type body_type;

		// Constructs a new collision body with the given arguments and returns a handle to it.
		template<typename ...Args>
		CollisionBodyHandle CreateBody(Args&&... args) {

			body_type body(std::forward<Args>(args)...);

			System::CollisionBodyMutator mutator(body);
			mutator.SetPhasePair(&BroadPhase(), &NarrowPhase());

			return AddBody(std::move(body));

		}

	protected:
		// Adds the body to the manager's body collection.
		virtual CollisionBodyHandle AddBody(body_type&& body) = 0;
		
	};

}