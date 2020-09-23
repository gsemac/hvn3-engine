#pragma once

#include "hvn3/collision/CollisionManager_.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/services/di_service_container.h"

namespace hvn3 {

	class PhysicsManager_ :
		public EventListenerBase<events::UpdateEvents> {

	public:
		HVN3_INJECT(PhysicsManager_(IEventManager& eventManager, ecs::ComponentManager& componentManager, CollisionManager_& collisionManager));

		~PhysicsManager_();

		void SetGravity(float value);
		void SetGravity(float x, float y);
		void SetGravity(const Vector2d& vector);

		void OnEvent(UpdateEventArgs& e) override;

	private:
		IEventManager* eventManager;
		ecs::ComponentManager* componentManager;
		CollisionManager_* collisionManager;
		Vector2d _gravity;

		void _updatePositions(UpdateEventArgs& e);

	};

}