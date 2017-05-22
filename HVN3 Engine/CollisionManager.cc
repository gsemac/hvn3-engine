#include <algorithm>
#include <utility>
#include "CollisionManager.h"
#include "Vector2d.h"
#include "NarrowPhaseCollisionManager.h"
#include "ICollidable.h"

namespace hvn3 {

	CollisionManager::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase) :
		_broadphase(std::move(broadphase)) {
	}

	void CollisionManager::ColliderAdd(ICollidable* collider) {

		if (!_broadphase)
			return;

		_broadphase->Add(collider);


	}
	void CollisionManager::ColliderRemove(ICollidable* collider) {

		if (!_broadphase)
			return;

		_broadphase->Remove(collider);

	}
	void CollisionManager::Clear() {

		if (!_broadphase)
			return;

		_broadphase->Clear();

	}
	void CollisionManager::Update(UpdateEventArgs& e) {

		// If the broadphase pointer is null, there is nothing to do.
		if (!_broadphase)
			return;

		// Update the state of the collision detection method.
		_broadphase->Update();

		// Process all collisions.
		ProcessCollisions(_broadphase->FindPairs());

	}

	bool CollisionManager::PlaceFree(ICollidable* collider, float x, float y) {

		// Create a vector to store the results.
		std::vector<ICollidable*> hits;

		// Get a list of all colliders that could potentially collide with the collider.
		_broadphase->QueryRegion(collider->AABB(), hits, collider->Filter().MaskBits());

		// If the list is empty, the place is free.
		if (hits.size() == 0)
			return true;

		for (size_t i = 0; i < hits.size(); ++i) {

			// Ignore self.
			if (hits[i] == collider)
				continue;

			// Check for a collision.
			if (NarrowPhaseCollisionManager::TestCollision(collider, x, y, hits[i], hits[i]->X(), hits[i]->Y()))
				return false;

		}

		// The collider did not collide with any other colliders.
		return true;

	}
	void CollisionManager::MoveContact(ICollidable* collider, float direction, int max_distance) {

		for (int i = 0; i < max_distance; ++i) {

			Point new_position = PointInDirection(Point(collider->X(), collider->Y()), direction, 1);

			if (!PlaceFree(collider, new_position.X(), new_position.Y()))
				break;

			collider->SetXY(new_position.X(), new_position.Y());

		}

	}

	void CollisionManager::ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const {
		return;
		// Test for a collision with each pair, and call the appropriate Collide function(s).
		for (auto i = pairs.begin(); i != pairs.end(); ++i) {
			ICollidable* a = i->first;
			ICollidable* b = i->second;
			if (!NarrowPhaseCollisionManager::TestCollision(a, b))
				continue;
			if (a->CollidesWith(b))
				a->Collide(b);
			if (b->CollidesWith(a))
				b->Collide(a);
		}

	}

}