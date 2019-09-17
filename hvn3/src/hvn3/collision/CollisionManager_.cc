#include "hvn3/collision/CollisionManager_.h"

#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/io/Console.h"

namespace hvn3 {

	// Public methods

	CollisionManager_::CollisionManager_() :
		_broadphase(32, 32),
		_debug_mode_enabled(true) {
	}

	void CollisionManager_::OnStart(StartEventArgs& e) {

		e.Context().Get<EventManager>()->Subscribe<events::UpdateEvents>(this, EventListenerPriority::COLLISIONS_UPDATE_PRIORITY);

		_context = e.Context();

		if (_debug_mode_enabled)
			SetDebugModeEnabled(true);

	}
	void CollisionManager_::OnEnd(EndEventArgs& e) {

		e.Context().Get<EventManager>()->UnsubscribeAll(this);

		_context = e.Context();

	}

	void CollisionManager_::OnEvent(UpdateEventArgs& e) {

		_refreshBroadphase();
		_refreshCollisions();

	}
	void CollisionManager_::OnEvent(DrawEventArgs& e) {

		_context.Get<ecs::ComponentManager>()->View<TransformComponent, ColliderComponent>().ForEach([&](TransformComponent& transform, ColliderComponent& collider) {

			RectangleF aabb = collider.Aabb().Translate(transform.Position());

			e.Graphics().DrawRectangle(aabb, Color::Red, 1.0f);

		});

	}

	void CollisionManager_::GetBroadphasePairs(std::vector<BroadphasePair>& output, bool ignoreCache) {

		if (ignoreCache)
			_refreshBroadphase();

		// Copy all broadphase pairs to the output buffer.

		output.reserve(_broadphase.IntersectingPairs().size());

		for (auto i = _broadphase.IntersectingPairs().begin(); i != _broadphase.IntersectingPairs().end(); ++i)
			output.push_back(BroadphasePair(*i->first, *i->second));

	}

	void CollisionManager_::SetDebugModeEnabled(bool value) {

		_debug_mode_enabled = value;

		if (_context) {

			if (_debug_mode_enabled)
				_context.Get<EventManager>()->Subscribe<events::DrawEvents>(this, EventListenerPriority::LOW_PRIORITY);
			else
				_context.Get<EventManager>()->Unsubscribe<events::DrawEvents>(this);

		}

	}

	void CollisionManager_::_refreshBroadphase() {

		// Update the state of the broadphase method.

		_broadphase.Clear();

		if (auto m = _context.Get<ecs::ComponentManager>()) {

			// Insert all colliders into the spatial partition.

			m->View<TransformComponent, ColliderComponent>().ForEach([this](TransformComponent& transform, ColliderComponent& collider) {

				RectangleF aabb = collider.Aabb().Translate(transform.Position());

				_broadphase.Insert(BroadphasePair::value_type(&transform, &collider), aabb);

			});

		}

	}
	void CollisionManager_::_refreshCollisions() {

		// Iterate over all broadphase pairs, and determine which collisions are actually occurring.
		// The broadphase takes care of checking AABB intersections.

		for (auto i = _broadphase.IntersectingPairs().begin(); i != _broadphase.IntersectingPairs().end(); ++i) {

			bool hit = TestIntersectionAt(i->first->collider->GetHitMask(), i->first->transform->Position(),
				i->second->collider->GetHitMask(), i->second->transform->Position());

			if (hit) {

				// ...

			}

			//ColliderComponent* body_1 = i->first->collider;
			//ColliderComponent* body_2 = i->second->collider;

			//CollisionResult m;

			//if (!Narrow().TestCollision(body_1, body_2, m))
			//	continue;

			//// Call the collision event for the first object.
			//if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
			//	body_1->OnCollide(body_2);

			//// Call the collision event for the second object.
			//if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
			//	body_2->OnCollide(body_1);

			//// Add the pair to the list of collisions.
			//this->GetPairs().push_back(m);

		}

	}

}