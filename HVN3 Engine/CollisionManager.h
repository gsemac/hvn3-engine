#pragma once
#include "ICollisionManager.h"
#include "ObjectCollisionBody.h"
#include "NarrowPhaseCollisionManager.h"
#include <memory>
#include <unordered_map>

namespace hvn3 {

	class Object;

	class CollisionManager final : public ICollisionManager<Object*> {

		typedef std::pair<key_type, ObjectCollisionBody> value_type;

	public:
		CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase_method);
		
		ICollisionBody* GetBody(key_type key) override;
		const ICollisionBody* GetBody(key_type key) const override;
		ICollisionBody* CreateBody(key_type key) override;
		bool RemoveBody(key_type key) override;
		void ClearAll() override;
		IBroadPhaseCollisionManager* BroadPhase() override;
		void OnUpdate(UpdateEventArgs& e) override;

		const std::vector<CollisionManifold>& CollidingPairs() const override;

		bool PlaceFree(ICollisionBody* body, const PointF& position) override;
		bool PlaceFreeIf(ICollisionBody* body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) override;
		bool MoveContact(ICollisionBody* body, float direction, float max_distance) override;
		bool MoveContactIf(ICollisionBody* body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) override;
		bool MoveOutside(ICollisionBody* body, float direction, float max_distance) override;
		bool MoveOutsideBody(ICollisionBody* body, ICollisionBody* other, float direction, float max_distance) override;

	private:
		// Pointer to the broad phrase collision management object.
		std::unique_ptr<IBroadPhaseCollisionManager> _broadphase_method;
		// The narrow phase collision management object.
		NarrowPhaseCollisionManager _narrowphase_method;
		// A collection of keys mapped to bodies.
		std::unordered_map<key_type, ObjectCollisionBody> _bodies;

		void CheckPairs(const IBroadPhaseCollisionManager::collider_pair_collection_type& pairs);

	};

}