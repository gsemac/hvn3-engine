#include "ObjectManager.h"
#include "ICollisionManager.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"
#include <utility>

namespace hvn3 {

	ObjectManager::ObjectManager(std::unique_ptr<ICollisionManager>& collision_manager) :
		_collision_manager(std::move(collision_manager)) {

		_last_found_id = noone;
		_last_found_index = 0;

	}
	void ObjectManager::InstanceAdd(std::shared_ptr<Object> object) {

		// If the object's id is non-null, add it to the collision manager.
		if (object->Id() != noone)
			CollisionManager()->ColliderAdd(object.get());

		_objects.push_back(object);

		//// If there are no objects in the list, just insert the new object.
		//if (_objects.size() == 0) {
		//	_objects.push_back(object);
		//	return;
		//}

		//// Get the highest and lowest depths in the list.
		//int lowest_depth = _objects.back()->Depth();
		//int highest_depth = _objects.front()->Depth();

		//if (object->Depth() <= lowest_depth) 
		//	// If the object's depth is <= than the lowest depth, insert last.
		//	_objects.push_back(object);

		//else if (object->Depth() >= highest_depth) 
		//	// If the objects depth is >= the highest depth, insert first.
		//	_objects.push_front(object);
		//
		//else {
		//	// Find a proper position for the object according to its depth.
		//	for (auto it = __objects.begin(); it != __objects.end(); ++it) {
		//		if ((*it)->Depth() < object->Depth()) {
		//			__objects.insert(it, object);
		//			return;
		//		}
		//	}
		//	__objects.push_back(object);
		//}

	}
	void ObjectManager::Clear() {

		// Clear all colliders from the collision manager.
		_collision_manager->Clear();

		// Clear all objects from the object manager.
		_objects.clear();

	}
	std::unique_ptr<ICollisionManager>& ObjectManager::CollisionManager() {

		return _collision_manager;

	}
	Object* ObjectManager::InstanceFind(ObjectId id) {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i]->Id() == id)
				return _objects[i].get();

		return nullptr;

	}
	Object* ObjectManager::InstanceFindNext(ObjectId id) {

		// If the object id we're looking for differs from the last one, reset the found index.
		if (_last_found_id != id)
			_last_found_index = 0;
		_last_found_id = id;

		// Find the next instance, starting at the last found index.
		for (size_t i = _last_found_index + 1; i < _objects.size(); ++i)
			if (_objects[i]->Id() == id)
				return _objects[i].get();

		// If we didn't find any more instances, reset the last found index and return nullptr.
		_last_found_index = 0;
		return nullptr;

	}
	size_t ObjectManager::InstanceCount() const {

		return _objects.size();

	}
	size_t ObjectManager::InstanceCount(ObjectId id) const {

		size_t count = 0;

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i]->Id() == id)
				++count;

		return count;

	}
	bool ObjectManager::InstanceExists(ObjectId id) const {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i]->Id() == id)
				return true;

		return false;

	}
	void ObjectManager::OnUpdate(UpdateEventArgs& e) {

		// Run the pre-update procedure for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnBeginUpdate(e);

		// Run the primary update procedure for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnUpdate(e);

		// Update the Collision Manager.
		_collision_manager->Update(e);

		// Run the post update procedure for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnEndUpdate(e);

	}
	void ObjectManager::OnDraw(DrawEventArgs& e) {

		// Draw all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnDraw(e);

	}

}