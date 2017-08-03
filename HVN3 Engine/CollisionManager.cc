#include <algorithm>
#include <utility>
#include "CollisionManager.h"
#include "Vector2d.h"
#include "NarrowPhaseCollisionManager.h"
#include "Object.h"
#include "Exception.h"

namespace hvn3 {
	namespace Collision {

		CollisionManager::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase_method) :
			_broadphase_method(std::move(broadphase_method)) {
		}

		ICollisionBody* CollisionManager::GetBody(key_type key) {

			return const_cast<ICollisionBody*>(static_cast<const CollisionManager*>(this)->GetBody(key));

		}
		const ICollisionBody* CollisionManager::GetBody(key_type key) const {

			//auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });
			auto it = _bodies.find(key);

			if (it == _bodies.end())
				return nullptr;

			return &it->second;

		}
		ICollisionBody* CollisionManager::CreateBody(key_type key) {

			auto result = _bodies.emplace(value_type(key, ObjectCollisionBody(key->Shared())));

			ICollisionBody* body = &result.first->second;

			if (result.second)
				_broadphase_method->AddBody(body);

			return body;

		}
		bool CollisionManager::RemoveBody(key_type key) {

			//auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });
			auto it = _bodies.find(key);

			if (it == _bodies.end())
				return false;

			_bodies.erase(it);
			_broadphase_method->RemoveBody(&it->second);

			return true;

		}
		void CollisionManager::ClearAll() {

			_broadphase_method->ClearAll();

			_bodies.clear();

		}

		IBroadPhaseCollisionManager* CollisionManager::BroadPhase() {

			return _broadphase_method.get();

		}

		void CollisionManager::OnUpdate(UpdateEventArgs& e) {

			// Update the state of the collision detection method.
			_broadphase_method->OnUpdate(e);

			// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
			CheckPairs(_broadphase_method->FindCandidatePairs());

		}

		const std::vector<CollisionManifold>& CollisionManager::CollidingPairs() const {

			return _collisions;

		}

		bool CollisionManager::PlaceFree(ICollisionBody* body, const PointF& position) {

			return PlaceFreeIf(body, position, [](ICollisionBody*) { return true; });

		}
		bool CollisionManager::PlaceFreeIf(ICollisionBody* body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) {

			// If the object does not have a collision mask, return true immediately (no collisions are possible).
			if (body->HitMask() == nullptr)
				return true;

			// Create a vector to store the results.
			IBroadPhaseCollisionManager::collider_collection_type hits;

			// Get a list of all colliders that could potentially collide with the collider.
			_broadphase_method->QueryRegion(body->AABB(), hits, body->Filter().MaskBits());

			// If the list is empty, the place is free.
			if (hits.size() == 0)
				return true;

			for (size_t i = 0; i < hits.size(); ++i) {

				// Ignore self and objects that don't meet the given condition.
				if (hits[i] == body || !condition(body))
					continue;

				// Check for a collision.
				CollisionManifold m;
				if (_narrowphase_method.TestCollision(body, position, hits[i], hits[i]->Position(), m))
					return false;

			}

			// The collider did not collide with any other colliders, so the plaec is free.
			return true;

		}
		bool CollisionManager::MoveContact(ICollisionBody* body, float direction, float max_distance) {

			return MoveContactIf(body, direction, max_distance, [](ICollisionBody*) { return true; });

		}
		bool CollisionManager::MoveContactIf(ICollisionBody* body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) {

			float distance = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			PointF new_position = Math::Geometry::PointInDirection(body->Position(), direction, distance_per_step);
			bool place_free;

			while ((place_free = PlaceFreeIf(body, new_position, condition), place_free) && distance < (std::abs)(max_distance)) {

				body->SetPosition(new_position);

				new_position = Math::Geometry::PointInDirection(new_position, direction, distance_per_step);

				distance += distance_per_step;

			}

			return !place_free;

		}
		bool CollisionManager::MoveOutside(ICollisionBody* body, float direction, float max_distance) {

			PointF pos = body->Position();
			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;

			while ((place_free = PlaceFree(body, pos), !place_free) && dist < max_distance) {

				pos = Math::Geometry::PointInDirection(pos, direction, distance_per_step);

				dist += distance_per_step;

			}

			body->SetPosition(pos);

			return place_free;

		}
		bool CollisionManager::MoveOutsideBody(ICollisionBody* body, ICollisionBody* other, float direction, float max_distance) {

			float dist = 0.0f;
			float distance_per_step = Math::Min(1.0f, max_distance);
			bool place_free;
			CollisionManifold m;

			while ((place_free = _narrowphase_method.TestCollision(body, other, m), place_free) && dist < (std::abs)(max_distance)) {

				body->SetPosition(Math::Geometry::PointInDirection(body->Position(), direction, distance_per_step));

				dist += distance_per_step;

			}

			return place_free;

		}

		void CollisionManager::CheckPairs(const IBroadPhaseCollisionManager::collider_pair_collection_type& pairs) {

			// Clear the list of colliding pairs.
			_collisions.clear();

			// Test for a collision with each pair and call the appropriate "on collision" function(s).
			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				ObjectCollisionBody* body_1 = static_cast<ObjectCollisionBody*>(i->first);
				ObjectCollisionBody* body_2 = static_cast<ObjectCollisionBody*>(i->second);

				CollisionManifold m;
				m.BodyA = body_1;
				m.BodyB = body_2;

				if (!_narrowphase_method.TestCollision(body_1, body_2, m))
					continue;

				if (body_1->ObjectExpired() || body_2->ObjectExpired()) {

					// If either object is expired, disregard the collision and remove the bodies.
					if (body_1->ObjectExpired())
						RemoveBody(body_1->GetObject());

					if (body_2->ObjectExpired())
						RemoveBody(body_2->GetObject());

					return;

				}

				// Call the collision event for the first object.
				if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
					body_1->GetObject()->OnCollision(CollisionEventArgs(body_2->GetObject(), body_2));

				// Call the collision event for the second object.
				if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
					body_2->GetObject()->OnCollision(CollisionEventArgs(body_1->GetObject(), body_2));

				// Add the pair to the list of collisions.
				_collisions.push_back(m);

			}

		}

	}
}