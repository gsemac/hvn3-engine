#pragma once
#include <memory>
#include "ICollisionManager.h"
#include "IBroadPhaseCollisionManager.h"
#include "NarrowPhaseCollisionManager.h"
#include "IObjectManagerListener.h"

namespace hvn3 {

	class CollisionManager : public ICollisionManager, public IObjectManagerListener {

	public:
		CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase_method);

		void AddObject(Object* object) override;
		void RemoveObject(Object* object) override;
		void ClearObjects() override;

		bool PlaceFree(Object* object, const PointF& position) override;
		bool PlaceFreeIf(Object* object, const PointF& position, const std::function<bool(Object*)>& condition) override;
		void MoveContact(Object* object, float direction, float max_distance) override;
		void MoveContactIf(Object* object, float direction, float max_distance, const std::function<bool(Object*)>& condition) override;
		void MoveOutside(Object* object, float direction, float max_distance) override;
		void MoveOutsideObject(Object* object, Object* other, float direction, float max_distance) override;

		IBroadPhaseCollisionManager& BroadPhase();

		void Update(UpdateEventArgs& e) override;

		void OnInstanceAdded(InstanceAddedEventArgs& e) override;
		void OnInstanceRemoved(InstanceRemovedEventArgs& e) override;
		void OnInstancesCleared(InstancesClearedEventArgs& e) override;
		void OnObjectManagerUpdate(UpdateEventArgs& e) override;

	private:
		std::unique_ptr<IBroadPhaseCollisionManager> _broadphase_method;
		NarrowPhaseCollisionManager _narrowphase_method;

		void CheckPairs(const IBroadPhaseCollisionManager::ColliderPairCollection& pairs) const;

	};

}