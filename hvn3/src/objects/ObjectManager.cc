#include "objects/ObjectManager.h"
#include "collision/ICollisionManager.h"
#include "core/DrawEventArgs.h"
#include "core/UpdateEventArgs.h"
#include "core/DestroyEventArgs.h"
#include <utility>
#include <algorithm>

namespace hvn3 {

	ObjectManager::ObjectManager() {

		_last_found_id = NoOne;
		_last_found_index = 0;

	}

	void ObjectManager::AddInstance(ObjectPtr& object) {

		//// Trigger all listeners.
		//for (size_t i = 0; i < _listeners.size(); ++i)
		//	_listeners[i]->OnInstanceAdded(InstanceAddedEventArgs(object.get()));

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
	void ObjectManager::AddInstance(Object* object) {

		AddInstance(ObjectPtr(object));

	}
	void ObjectManager::ClearAll() {

		// Clear all objects from the object manager.
		_objects.clear();

	}
	void ObjectManager::DestroyAll() {

		//// Trigger all listeners.
		//for (size_t i = 0; i < _listeners.size(); ++i)
		//	_listeners[i]->OnInstancesCleared(InstancesClearedEventArgs());

		// Call the destroy event for each object.
		DestroyEventArgs e;
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			(*it)->OnDestroy(e);

		// Clear all objects.
		ClearAll();

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

	void ObjectManager::OnBeginUpdate(UpdateEventArgs& e) {

		bool removed = false;

		// Call the begin update event for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it) {

			if ((*it)->IsDestroyed()) {
				removed = true;
				(*it)->OnDestroy(DestroyEventArgs());
			}
			else if ((*it)->IsActive())
				(*it)->OnBeginUpdate(e);

		}

		// If any objects were destroyed, remove them from the collection.
		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());

	}
	void ObjectManager::OnUpdate(UpdateEventArgs& e) {

		bool removed = false;

		// Call the update event for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it) {

			if ((*it)->IsDestroyed()) {
				removed = true;
				(*it)->OnDestroy(DestroyEventArgs());
			}
			else if ((*it)->IsActive())
				(*it)->OnUpdate(e);

		}

		// If any objects were destroyed, remove them from the collection.
		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());

		//// Trigger all listeners.
		//for (size_t i = 0; i < _listeners.size(); ++i)
		//	_listeners[i]->OnObjectManagerUpdate(e);

	}
	void ObjectManager::OnEndUpdate(UpdateEventArgs& e) {

		bool removed = false;

		// Call the end update event for all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it) {

			if ((*it)->IsDestroyed()) {
				removed = true;
				(*it)->OnDestroy(DestroyEventArgs());
			}
			else if ((*it)->IsActive())
				(*it)->OnEndUpdate(e);

		}

		// If any objects were destroyed, remove them from the collection.
		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());

	}
	void ObjectManager::OnDraw(DrawEventArgs& e) {

		// Draw all objects.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if ((*it)->IsActive())
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


	void ObjectManager::_removeDestroyedObjects(std::vector<ObjectPtr>::iterator begin, std::vector<ObjectPtr>::iterator end) {

		std::remove_if(begin, end, [](const ObjectPtr& obj) { return obj->IsDestroyed(); });

	}

}