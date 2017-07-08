#include <algorithm>
#include <utility>
#include "CollisionManager.h"
#include "Vector2d.h"
#include "NarrowPhaseCollisionManager.h"
#include "Collider.h"
#include "Object.h"

namespace hvn3 {

	CollisionManager::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase_method) :
		_broadphase_method(std::move(broadphase_method)) {
	}

	ICollisionBody* CollisionManager::GetBody(key_type key) {

		return const_cast<ICollisionBody*>(static_cast<const CollisionManager*>(this)->GetBody(key));

	}
	const ICollisionBody* CollisionManager::GetBody(key_type key) const {

		auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });

		if (it == _bodies.end())
			return nullptr;

		return &it->second;

	}
	ICollisionBody* CollisionManager::CreateBody(key_type key) {

		_bodies.emplace_back(value_type(key, ObjectCollisionBody(key)));

		ICollisionBody* body = &(--_bodies.end())->second;

		_broadphase_method->AddBody(body);

		return body;

	}
	bool CollisionManager::RemoveBody(key_type key) {

		auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });

		if (it == _bodies.end())
			return false;

		_bodies.erase(it);
		_broadphase_method->RemoveBody(&it->second);

		return true;

	}
	void CollisionManager::ClearBodies() {

		_broadphase_method->ClearBodies();

		_bodies.clear();

	}

	IBroadPhaseCollisionManager* CollisionManager::BroadPhase() {

		return _broadphase_method.get();

	}

	void CollisionManager::OnUpdate(UpdateEventArgs& e) {

		// Update the state of the collision detection method.
		_broadphase_method->OnUpdate(e);

		// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
		CheckPairs(_broadphase_method->FindPairs());

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
			if (_narrowphase_method.TestCollision(body, body->Position(), hits[i], hits[i]->Position()))
				return false;

		}

		// The collider did not collide with any other colliders, so the plaec is free.
		return true;

	}
	bool CollisionManager::MoveContact(ICollisionBody* body, float direction, float max_distance) {

		return MoveContactIf(body, direction, max_distance, [](ICollisionBody*) { return true; });

	}
	bool CollisionManager::MoveContactIf(ICollisionBody* body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) {

		PointF pos = body->Position();
		float dist = 0.0f;
		float distance_per_step = 1.0f;
		bool place_free;

		while ((place_free = PlaceFreeIf(body, pos, condition), place_free) && dist < max_distance) {

			body->SetPosition(pos);

			pos = PointInDirection(pos, direction, distance_per_step);

			dist += distance_per_step;

		}

		return !place_free;

	}
	bool CollisionManager::MoveOutside(ICollisionBody* body, float direction, float max_distance) {

		PointF pos = body->Position();
		float dist = 0.0f;
		float distance_per_step = 1.0f;
		bool place_free;

		while ((place_free = PlaceFree(body, pos), !place_free) && dist < max_distance) {

			pos = PointInDirection(pos, direction, distance_per_step);

			dist += distance_per_step;

		}

		body->SetPosition(pos);

		return place_free;

	}
	bool CollisionManager::MoveOutsideBody(ICollisionBody* body, ICollisionBody* other, float direction, float max_distance) {

		float dist = 0.0f;
		float distance_per_step = 1.0f;
		bool place_free;

		while ((place_free = _narrowphase_method.TestCollision(body, other), place_free) && dist < max_distance) {

			body->SetPosition(PointInDirection(body->Position(), direction, distance_per_step));

			dist += distance_per_step;

		}

		return place_free;

	}

	void CollisionManager::CheckPairs(const IBroadPhaseCollisionManager::collider_pair_collection_type& pairs) const {

		// Test for a collision with each pair and call the appropriate "on collision" function(s).
		for (auto i = pairs.begin(); i != pairs.end(); ++i) {

			ObjectCollisionBody* body_1 = static_cast<ObjectCollisionBody*>(i->first);
			ObjectCollisionBody* body_2 = static_cast<ObjectCollisionBody*>(i->second);

			if (!_narrowphase_method.TestCollision(body_1, body_2))
				continue;

			if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
				body_1->GetObject()->OnCollision(CollisionEventArgs(body_2->GetObject(), body_2));

			if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
				body_2->GetObject()->OnCollision(CollisionEventArgs(body_1->GetObject(), body_2));

		}

	}

}