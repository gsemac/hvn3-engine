//#include <algorithm>
//#include <utility>
//#include "collision/CollisionManager.h"
//#include "math/Vector2d.h"
//#include "collision/NarrowPhaseCollisionManager.h"
//#include "objects/Object.h"
//#include "exceptions/Exception.h"
//
//namespace hvn3 {
//
//	CollisionManager::CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>& broadphase_method) :
//		_broadphase_method(std::move(broadphase_method)) {
//	}
//
//	ICollisionBody* CollisionManager::GetBody(key_type key) {
//
//		return const_cast<ICollisionBody*>(static_cast<const CollisionManager*>(this)->GetBody(key));
//
//	}
//	const ICollisionBody* CollisionManager::GetBody(key_type key) const {
//
//		//auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });
//		auto it = _bodies.find(key);
//
//		if (it == _bodies.end())
//			return nullptr;
//
//		return &it->second;
//
//	}
//	ICollisionBody* CollisionManager::CreateBody(key_type key) {
//
//		auto result = _bodies.emplace(value_type(key, ObjectCollisionBody(key->Shared())));
//
//		ICollisionBody* body = &result.first->second;
//
//		if (result.second)
//			_broadphase_method->AddBody(body);
//
//		return body;
//
//	}
//	bool CollisionManager::RemoveBody(key_type key) {
//
//		//auto it = std::find_if(_bodies.begin(), _bodies.end(), [&](const value_type& pair) { return pair.first == key; });
//		auto it = _bodies.find(key);
//
//		if (it == _bodies.end())
//			return false;
//
//		_bodies.erase(it);
//		_broadphase_method->RemoveBody(&it->second);
//
//		return true;
//
//	}
//	void CollisionManager::ClearAll() {
//
//		_broadphase_method->ClearAll();
//
//		_bodies.clear();
//
//	}
//
//	IBroadPhaseCollisionManager* CollisionManager::BroadPhase() {
//
//		return _broadphase_method.get();
//
//	}
//
//	void CollisionManager::OnUpdate(UpdateEventArgs& e) {
//
//		// Update the state of the collision detection method.
//		_broadphase_method->OnUpdate(e);
//
//		// Get a vector containing all potentially-colliding pairs from the broadphase method, and check all collisions.
//		CheckPairs(_broadphase_method->FindCandidatePairs());
//
//	}
//
//	const std::vector<CollisionManifold>& CollisionManager::CollidingPairs() const {
//
//		return _collisions;
//
//	}
//
//	bool CollisionManager::PlaceFree(ICollisionBody* body, const PointF& position) {
//
//	
//
//	}
//	bool CollisionManager::PlaceFreeIf(ICollisionBody* body, const PointF& position, const std::function<bool(ICollisionBody*)>& condition) {
//
//		
//	}
//	bool CollisionManager::MoveContact(ICollisionBody* body, float direction, float max_distance) {
//
//	
//
//	}
//	bool CollisionManager::MoveContactIf(ICollisionBody* body, float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) {
//
//		
//
//	}
//	bool CollisionManager::MoveOutside(ICollisionBody* body, float direction, float max_distance) {
//
//	
//
//	}
//	bool CollisionManager::MoveOutsideBody(ICollisionBody* body, ICollisionBody* other, float direction, float max_distance) {
//
//		
//
//	}
//
//	void CollisionManager::CheckPairs(const IBroadPhaseCollisionManager::collider_pair_collection_type& pairs) {
//
//		// Clear the list of colliding pairs.
//		_collisions.clear();
//
//		// Test for a collision with each pair and call the appropriate "on collision" function(s).
//		for (auto i = pairs.begin(); i != pairs.end(); ++i) {
//
//			ObjectCollisionBody* body_1 = static_cast<ObjectCollisionBody*>(i->first);
//			ObjectCollisionBody* body_2 = static_cast<ObjectCollisionBody*>(i->second);
//
//			CollisionManifold m;
//			m.BodyA = body_1;
//			m.BodyB = body_2;
//
//			if (!_narrowphase_method.TestCollision(body_1, body_2, m))
//				continue;
//
//			if (body_1->ObjectExpired() || body_2->ObjectExpired()) {
//
//				// If either object is expired, disregard the collision and remove the bodies.
//				if (body_1->ObjectExpired())
//					RemoveBody(body_1->GetObject());
//
//				if (body_2->ObjectExpired())
//					RemoveBody(body_2->GetObject());
//
//				return;
//
//			}
//
//			// Call the collision event for the first object.
//			if (body_1->Filter().MaskBits() & body_2->Filter().CategoryBits())
//				body_1->GetObject()->OnCollision(CollisionEventArgs(body_2->GetObject(), body_2));
//
//			// Call the collision event for the second object.
//			if (body_2->Filter().MaskBits() & body_1->Filter().CategoryBits())
//				body_2->GetObject()->OnCollision(CollisionEventArgs(body_1->GetObject(), body_2));
//
//			// Add the pair to the list of collisions.
//			_collisions.push_back(m);
//
//		}
//
//	}
//
//}