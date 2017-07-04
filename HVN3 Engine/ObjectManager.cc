#include "ObjectManager.h"
#include "ICollisionManager.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"
#include "IObjectManagerListener.h"
#include <utility>
#include <algorithm>

namespace hvn3 {

	ObjectManager::ObjectManager() {

		_last_found_id = NoOne;
		_last_found_index = 0;

	}

	void ObjectManager::AddInstance(ObjectPtr& object) {

		// Trigger all listeners.
		for (size_t i = 0; i < _listeners.size(); ++i)
			_listeners[i]->OnInstanceAdded(InstanceAddedEventArgs(object.get()));

		// Add the object to our object collection.
		_objects.push_back(object);

		// Call the object's create event.
		object->OnCreate(CreateEventArgs());

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

		//// Clear all colliders from the collision manager.
		//_collision_manager->ClearObjects();

		// Trigger all listeners.
		for (size_t i = 0; i < _listeners.size(); ++i)
			_listeners[i]->OnInstancesCleared(InstancesClearedEventArgs());

		// Clear all objects from the object manager.
		_objects.clear();

	}
	Object* ObjectManager::FindInstance(ObjectId id) {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i]->Id() == id)
				return _objects[i].get();

		return nullptr;

	}
	Object* ObjectManager::FindNextInstance(ObjectId id) {

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

		//// Update the Collision Manager.
		//_collision_manager->Update(e);

		// Trigger all listeners.
		for (size_t i = 0; i < _listeners.size(); ++i)
			_listeners[i]->OnObjectManagerUpdate(e);

		// Run the post update procedure for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnEndUpdate(e);

	}
	void ObjectManager::OnDraw(DrawEventArgs& e) {

		// Draw all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnDraw(e);

	}

	void ObjectManager::AddListener(IObjectManagerListener* listener) {

		_listeners.push_back(listener);

	}
	void ObjectManager::RemoveListener(IObjectManagerListener* listener) {

		auto position = std::find(_listeners.begin(), _listeners.end(), listener);
		
		if (position != _listeners.end())
			_listeners.erase(position);

	}

}