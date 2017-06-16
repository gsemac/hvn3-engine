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

	void CollisionManager::AddObject(Object* object) {

		_broadphase_method->AddCollider(&object->Collider());


	}
	void CollisionManager::RemoveObject(Object* object) {

		_broadphase_method->RemoveCollider(&object->Collider());

	}
	void CollisionManager::ClearObjects() {

		_broadphase_method->ClearColliders();

	}

	void CollisionManager::Update(UpdateEventArgs& e) {

		// Update the state of the collision detection method.
		_broadphase_method->Update();

		// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
		CheckPairs(_broadphase_method->FindPairs());

	}

	bool CollisionManager::PlaceFree(Object* object, const PointF& position) {

		// Create a vector to store the results.
		IBroadPhaseCollisionManager::ColliderCollection hits;

		// Get a list of all colliders that could potentially collide with the collider.
		_broadphase_method->QueryRegion(object->Collider().AABB(), hits, object->Collider().Filter().MaskBits());

		// If the list is empty, the place is free.
		if (hits.size() == 0)
			return true;

		for (size_t i = 0; i < hits.size(); ++i) {

			// Ignore self.
			if (hits[i] == &object->Collider())
				continue;

			// Get object pointer to the hit.
			Object* hit_obj = (Object*)hits[i]->TrackingObject();

			// Check for a collision.
			if (_narrowphase_method.TestCollision(&object->Collider(), object->Position(), hits[i], hit_obj->Position()))
				return false;

		}

		// The collider did not collide with any other colliders, so the plaec is free.
		return true;

	}
	void CollisionManager::MoveContact(Object* object, float direction, int max_distance) {

		for (int i = 0; i < max_distance; ++i) {

			PointF new_position = PointInDirection(PointF(object->X(), object->Y()), direction, 1);

			if (!PlaceFree(object, new_position))
				break;

			object->SetPosition(new_position);

		}

	}

	void CollisionManager::CheckPairs(const IBroadPhaseCollisionManager::ColliderPairCollection& pairs) const {
	
		// Test for a collision with each pair and call the appropriate "on collision" function(s).
		for (auto i = pairs.begin(); i != pairs.end(); ++i) {

			Collider* a = i->first;
			Collider* b = i->second;

			Object* a_obj = (Object*)a->TrackingObject();
			Object* b_obj = (Object*)b->TrackingObject();

			if (!_narrowphase_method.TestCollision(a, b))
				continue;

			if (a->Filter().MaskBits() & b->Filter().CategoryBits())
				a_obj->OnCollision(CollisionEventArgs(b_obj));

			if (b->Filter().MaskBits() & a->Filter().CategoryBits())
				b_obj->OnCollision(CollisionEventArgs(a_obj));

		}

	}

}