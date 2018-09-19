#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/objects/Object.h"
#include "hvn3/objects/ObjectEventArgs.h"
#include "hvn3/objects/ObjectManager.h"
#include <algorithm>
#include <cassert>
#include <utility>

namespace hvn3 {

	ObjectManager::ObjectListItem::ObjectListItem(const IObjectPtr& object) :
		object(object) {

		callOnCreateEvent = true;
		callOnDestroyEvent = true;

	}
	ObjectManager::ObjectListItem::ObjectListItem(ObjectListItem&& other) {

		callOnCreateEvent = other.callOnCreateEvent;
		callOnDestroyEvent = other.callOnDestroyEvent;
		object = std::move(other.object);

	}



	ObjectManager::ObjectManager() {

		_last_found_id = NoOne;
		_last_found_index = 0;

	}


	void ObjectManager::Add(const IObjectPtr& object) {

		assert(static_cast<bool>(object));

		object->SetContext(_context);

		//// Trigger all listeners.
		//for (size_t i = 0; i < _listeners.size(); ++i)
		//	_listeners[i]->OnInstanceAdded(InstanceAddedEventArgs(object.get()));

		// Add the object to our object collection.
		_objects.emplace_back(ObjectListItem(object));

		//return _objects.back().object.get();

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
	void ObjectManager::Add(IObject* object) {

		assert(object != nullptr);

		Add(IObjectPtr(object));

	}
	void ObjectManager::Clear() {

		// Clear all Objects from the Object Manager without calling any events.
		_objects.clear();

	}
	void ObjectManager::DestroyAll() {

		// Destroy each Object. They will be removed with the next update.
		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if (!it->object->IsDestroyed())
				it->object->Destroy();

	}
	IObject* ObjectManager::Find(ObjectId id) {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return _objects[i].object.get();

		return nullptr;

	}
	IObject* ObjectManager::FindNext(ObjectId id) {

		// If the object id we're looking for differs from the last one, reset the found index.
		if (_last_found_id != id)
			_last_found_index = 0;
		else
			++_last_found_index;

		_last_found_id = id;

		// Find the next instance, starting at the last found index.
		for (size_t i = _last_found_index; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return _objects[i].object.get();

		// If we didn't find anything, start back at the beginning of the collection.
		if (_last_found_index > 0)
			for (size_t i = 0; i < _last_found_index; ++i)
				if (_objects[i].object->Id() == id)
					return _objects[i].object.get();

		// If we didn't find any more instances, reset the last found index and return nullptr.
		_last_found_id = NoOne;
		_last_found_index = 0;

		return nullptr;

	}
	size_t ObjectManager::Count() const {

		return _objects.size();

	}
	size_t ObjectManager::Count(ObjectId id) const {

		size_t count = 0;

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				++count;

		return count;

	}
	bool ObjectManager::Exists(ObjectId id) const {

		for (size_t i = 0; i < _objects.size(); ++i)
			if (_objects[i].object->Id() == id)
				return true;

		return false;

	}
	void ObjectManager::ForEach(const std::function<void(IObjectPtr&)>& func) {

		for (auto i = _objects.begin(); i != _objects.end(); ++i)
			func(i->object);

	}
	void ObjectManager::ForEach(const std::function<void(const IObjectPtr&)>& func) const {

		for (auto i = _objects.begin(); i != _objects.end(); ++i)
			func(i->object);

	}

	void ObjectManager::OnBeginUpdate(UpdateEventArgs& e) {

		bool removed = false;

		for (size_t i = 0; i < _objects.size(); ++i) {
			std::cout << i << std::endl;
			if (_updateAndCheckObject(i, e))
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
		
			if (_updateAndCheckObject(i, e))
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

			if (_updateAndCheckObject(i, e))
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


	void ObjectManager::OnContextChanged(ContextChangedEventArgs& e) {

		_context = e.Context();

		// Update the context for all objects.
		for (auto i = _objects.begin(); i != _objects.end(); ++i)
			i->object->SetContext(_context);

	}


	void ObjectManager::_removeDestroyedObjects(object_list_type::iterator begin, object_list_type::iterator end) {

		_objects.erase(std::remove_if(begin, end, [](const ObjectListItem& item) { return item.object->IsDestroyed(); }), _objects.end());

	}
	bool ObjectManager::_updateAndCheckObject(size_t item_index, UpdateEventArgs& e) {
		
		// This method takes an index rather than a reference to item because there's no guarantee the vector won't be resized in in OnCreate or OnDestroy.
		// Passing a reference risks that reference being invalidated after the resize.
		
		// Call the OnCreate event for the Object if it hasn't been called yet.
		if (_objects[item_index].callOnCreateEvent) {
			_objects[item_index].object->OnCreate(CreateEventArgs());
			_objects[item_index].callOnCreateEvent = false;
		}

		if (_objects[item_index].object->IsDestroyed()) {
			if (_objects[item_index].callOnDestroyEvent) {
				_objects[item_index].object->OnDestroy(DestroyEventArgs());
				_objects[item_index].callOnDestroyEvent = false;
			}
		}
		else if (_objects[item_index].object->IsActive())
			return true;

		// If the Object is not active, its update events should not be called.
		return false;

	}

}