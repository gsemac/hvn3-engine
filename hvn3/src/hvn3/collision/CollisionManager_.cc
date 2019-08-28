#include "hvn3/collision/CollisionManager_.h"

#include "hvn3/components/ColliderComponent.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/io/Console.h"

namespace hvn3 {

	// Public methods

	void CollisionManager_::OnStart(StartEventArgs& e) {

		if (auto m = e.Context().Get<EventManager>())
			m->SubscribeAll(this);

	}
	void CollisionManager_::OnEnd(EndEventArgs& e) {

		if (auto m = e.Context().Get<EventManager>())
			m->UnsubscribeAll(this);

	}

	void CollisionManager_::OnEvent(UpdateEventArgs& e) {

		if (auto m = e.Context().Get<ecs::ComponentManager>()) {

			m->View<TransformComponent, ColliderComponent>().ForEach([](TransformComponent& transform, ColliderComponent& collider) {

			});

		}

	}

}