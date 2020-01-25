#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/components/PhysicsComponent.h"
#include "hvn3/physics/PhysicsManager_.h"

namespace hvn3 {

	// Public methods

	PhysicsManager_::PhysicsManager_(IEventManager& eventManager, ecs::ComponentManager& componentManager, CollisionManager_& collisionManager) :
		eventManager(&eventManager),
		componentManager(&componentManager),
		collisionManager(&collisionManager) {

		SetGravity(9.81f);

		this->eventManager->GetListenerRegistry().SubscribeAll(this, EventListenerPriority::PHYSICS_UPDATE_PRIORITY);

	}

	PhysicsManager_::~PhysicsManager_() {

		this->eventManager->GetListenerRegistry().UnsubscribeAll(this);

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

	void PhysicsManager_::OnEvent(UpdateEventArgs& e) {

		_updatePositions(e);

	}

	// Private methods

	void PhysicsManager_::_updatePositions(UpdateEventArgs& e) {

		// Update the positions of all physics bodies.

		componentManager->View<TransformComponent, PhysicsComponent>().ForEach([&, this](TransformComponent& transform, PhysicsComponent& body) {

			body.SetLinearVelocity(body.LinearVelocity() + _gravity * e.Delta());

			transform.SetPosition(transform.Position() + body.LinearVelocity());

			});

		// Get updated collisions.

		auto& pairs = collisionManager->GetBroadphasePairs(true);

		// Resolve collisions.

		for (auto i = pairs.begin(); i != pairs.end(); ++i) {

			// ...

		}

	}

}