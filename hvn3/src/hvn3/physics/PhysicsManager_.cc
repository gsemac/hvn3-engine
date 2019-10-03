#include "hvn3/collision/CollisionManager_.h"
#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/components/PhysicsComponent.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/physics/PhysicsManager_.h"

namespace hvn3 {

	// Public methods

	PhysicsManager_::PhysicsManager_() {

		SetGravity(9.81f);

	}

	void PhysicsManager_::SetGravity(float value) {
		SetGravity(0.0f, value);
	}
	void PhysicsManager_::SetGravity(float x, float y) {
		SetGravity(Vector2d(x, y));
	}
	void PhysicsManager_::SetGravity(const Vector2d& vector) {
		_gravity = vector;
	}

	void PhysicsManager_::OnStart(StartEventArgs& e) {

		e.Context().Get<EventManager>()->SubscribeAll(this, EventListenerPriority::PHYSICS_UPDATE_PRIORITY);

	}
	void PhysicsManager_::OnEnd(EndEventArgs& e) {

		e.Context().Get<EventManager>()->UnsubscribeAll(this);

	}
	void PhysicsManager_::OnEvent(UpdateEventArgs& e) {

		_updatePositions(e);

	}

	// Private methods

	void PhysicsManager_::_updatePositions(UpdateEventArgs& e) {

		if (auto componentManager = e.Context().Get<ecs::ComponentManager>()) {

			// Update the positions of all physics bodies.

			componentManager->View<TransformComponent, PhysicsComponent>().ForEach([&, this](TransformComponent& transform, PhysicsComponent& body) {

				body.SetLinearVelocity(body.LinearVelocity() + _gravity * e.Delta());

				transform.SetPosition(transform.Position() + body.LinearVelocity());

			});

			if (auto collisionManager = e.Context().Get<CollisionManager_>()) {

				// Get updated collisions.

				auto& pairs = collisionManager->GetBroadphasePairs(true);

				// Resolve collisions.

				for (auto i = pairs.begin(); i != pairs.end(); ++i) {

					// ...

				}

			}

		}

	}

}