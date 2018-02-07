#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/objects/Object.h"
#include "hvn3/objects/ObjectEventArgs.h"
#include "hvn3/objects/ObjectManager.h"
#include <utility>
#include <algorithm>

namespace hvn3 {

	ObjectManager::ObjectListItem::ObjectListItem(ObjectPtr& object) :
		object(object) {

		callOnCreateEvent = true;
		callOnDestroyEvent = true;

	}



	ObjectManager::ObjectManager() {

		_last_found_id = NoOne;
		_last_found_index = 0;

	}


	void ObjectManager::AddInstance(ObjectPtr& object) {

		//// Trigger all listeners.
		//for (size_t i = 0; i < _listeners.size(); ++i)
		//	_listeners[i]->OnInstanceAdded(InstanceAddedEventArgs(object.get()));

		// Add the object to our object collection.
		_objects.emplace_back(ObjectListItem(object));

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

		// Clear all Objects from the Object Manager without calling any events.
		_objects.clear();

	}
	void ObjectManager::DestroyAll() {

		// Destroy each Object. They will be removed with the next update.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if (!it->object->IsDestroyed())
				it->object->Destroy();

	}
	Object* ObjectManager::FindInstance(ObjectId id) {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return _objects[i].object.get();

		return nullptr;

	}
	Object* ObjectManager::FindNextInstance(ObjectId id) {

		// If the object id we're looking for differs from the last one, reset the found index.
		if (_last_found_id != id)
			_last_found_index = 0;
		_last_found_id = id;

		// Find the next instance, starting at the last found index.
		for (size_t i = _last_found_index + 1; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return _objects[i].object.get();

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
			if (_objects[i].object->Id() == id)
				++count;

		return count;

	}
	bool ObjectManager::InstanceExists(ObjectId id) const {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return true;

		return false;

	}

	void ObjectManager::OnBeginUpdate(UpdateEventArgs& e) {

		bool removed = false;

		for (size_t i = 0; i < _objects.size(); ++i) {

			if (_updateAndCheckObject(_objects[i], e))
				_objects[i].object->OnBeginUpdate(e);

			if (_objects[i].object->IsDestroyed())
				removed = true;
		}

		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());
		
	}
	void ObjectManager::OnUpdate(UpdateEventArgs& e) {

		bool removed = false;

		for (size_t i = 0; i < _objects.size(); ++i) {

			if (_updateAndCheckObject(_objects[i], e))
				_objects[i].object->OnUpdate(e);

			if (_objects[i].object->IsDestroyed())
				removed = true;
		}

		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());

	}
	void ObjectManager::OnEndUpdate(UpdateEventArgs& e) {

		bool removed = false;

		for (size_t i = 0; i < _objects.size(); ++i) {

			if (_updateAndCheckObject(_objects[i], e))
				_objects[i].object->OnEndUpdate(e);

			if (_objects[i].object->IsDestroyed())
				removed = true;
		}

		if (removed)
			_removeDestroyedObjects(_objects.begin(), _objects.end());

	}
	void ObjectManager::OnDraw(DrawEventArgs& e) {

		// Draw all objects.

		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			// Don't draw any objects that haven't had their create event called yet, or inactive objects.
			if (!it->callOnCreateEvent && it->object->IsActive())
				it->object->OnDraw(e);

	}

	void ObjectManager::AddListener(IObjectManagerListener* listener) {

		_listeners.push_back(listener);

	}
	void ObjectManager::RemoveListener(IObjectManagerListener* listener) {

		auto position = std::find(_listeners.begin(), _listeners.end(), listener);

		if (position != _listeners.end())
			_listeners.erase(position);

	}



	void ObjectManager::_removeDestroyedObjects(object_list_type::iterator begin, object_list_type::iterator end) {

		_objects.erase(std::remove_if(begin, end, [](const ObjectListItem& item) { return item.object->IsDestroyed(); }), _objects.end());

	}
	bool ObjectManager::_updateAndCheckObject(ObjectListItem& item, UpdateEventArgs& e) {

		// Call the OnCreate event for the Object if it hasn't been called yet.
		if (item.callOnCreateEvent) {
			item.object->OnCreate(CreateEventArgs(&e.Game()));
			item.callOnCreateEvent = false;
		}

		if (item.object->IsDestroyed()) {
			if (item.callOnDestroyEvent) {
				item.object->OnDestroy(DestroyEventArgs(&e.Game()));
				item.callOnDestroyEvent = false;
			}
		}
		else if (item.object->IsActive())
			return true;

		// If the Object is not active, its update events should not be called.
		return false;

	}

}