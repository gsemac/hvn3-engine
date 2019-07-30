#include "hvn3/objects/ObjectManager.h"

#include <utility>

namespace hvn3 {

	// ObjectManager::ObjectInfo

	// ObjectManager::ObjectInfo

	// Public methods

	ObjectManager::ObjectInfo::ObjectInfo(std::shared_ptr<IObject>&& object) :
		object(std::move(object)) {

		createEventCalled = false;
		destroyEventCalled = false;

	}
	ObjectManager::ObjectInfo::ObjectInfo(ObjectInfo&& other) {

		createEventCalled = other.createEventCalled;
		destroyEventCalled = other.destroyEventCalled;
		object = std::move(other.object);

	}

	// ObjectManager

	// Public methods

	ObjectManager::ObjectManager() {

		_update_start_index = 0;

	}

	void ObjectManager::OnStart(StartEventArgs& e) {

		_context = e.Context();

	}
	void ObjectManager::OnEnd(EndEventArgs& e) {

		_context = ApplicationContext();

	}

	void ObjectManager::Clear() {

		// Clear all objects without calling OnDestroy.
		_objects.clear();

	}
	void ObjectManager::DestroyAll() {

		// Flag all objects as destroyed; OnDestroy will be called next update.

		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if (!it->object->IsDestroyed())
				it->object->Destroy();

	}

	ObjectHandle ObjectManager::Find(IObject::object_id id) {

		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if (it->object->Id() == id)
				return ObjectHandle(std::weak_ptr<IObject>(it->object));

		return ObjectHandle();

	}

	ObjectManager::size_type ObjectManager::Count() const {

		return _objects.size();

	}
	ObjectManager::size_type ObjectManager::Count(IObject::object_id id) const {

		return std::count_if(_objects.begin(), _objects.end(), [=](const ObjectInfo& info) {
			return info.object->Id() == id;
		});

	}
	bool ObjectManager::Exists(IObject::object_id id) const {

		return std::any_of(_objects.begin(), _objects.end(), [=](const ObjectInfo& info) {
			return info.object->Id() == id;
		});

	}

	// Protected methods

	ApplicationContext ObjectManager::Context() {
		return _context;
	}

}

//#include "hvn3/core/DrawEventArgs.h"
//#include "hvn3/core/UpdateEventArgs.h"
//#include "hvn3/objects/Object.h"
//#include "hvn3/objects/ObjectManager.h"
//
//#include <algorithm>
//#include <cassert>
//#include <utility>
//
//namespace hvn3 {
//
//	// ObjectManager
//
//	// Public methods
//
//	ObjectManager::ObjectManager() {
//
//		_last_found_id = IObject::NULL_OBJECT_ID;
//		_last_found_index = 0;
//		_resort_required = false;
//
//	}
//
//	void ObjectManager::Clear() {
//
//		// Clear all Objects from the Object Manager without calling any events.
//		_objects.clear();
//
//	}
//	void ObjectManager::DestroyAll() {
//
//		// Destroy each Object. They will be removed with the next update.
//
//		for (auto it = _objects.begin(); it != _objects.end(); ++it)
//			if (!it->object->IsDestroyed())
//				it->object->Destroy();
//
//	}
//
//	IObject* ObjectManager::Find(IObject::object_id id) {
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			if (i->object->Id() == id)
//				return i->object.get();
//
//		return nullptr;
//
//	}
//	IObject* ObjectManager::FindNext(IObject::object_id id) {
//
//		// If the object id we're looking for differs from the last one, reset the found index.
//		if (_last_found_id != id)
//			_last_found_index = 0;
//		else
//			++_last_found_index;
//
//		_last_found_id = id;
//
//		// Find the next instance, starting at the last found index.
//		for (size_t i = _last_found_index; i < _objects.size(); ++i)
//			if (_objects[i].object->Id() == id)
//				return _objects[i].object.get();
//
//		// If we didn't find anything, start back at the beginning of the collection.
//		if (_last_found_index > 0)
//			for (size_t i = 0; i < _last_found_index; ++i)
//				if (_objects[i].object->Id() == id)
//					return _objects[i].object.get();
//
//		// If we didn't find any more instances, reset the last found index and return nullptr.
//		_last_found_id = IObject::NULL_OBJECT_ID;
//		_last_found_index = 0;
//
//		return nullptr;
//
//	}
//
//	size_t ObjectManager::Count() const {
//		return _objects.size();
//	}
//	size_t ObjectManager::Count(IObject::object_id id) const {
//
//		size_t count = 0;
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			if (i->object->Id() == id)
//				++count;
//
//		return count;
//
//	}
//	bool ObjectManager::Exists(IObject::object_id id) const {
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			if (i->object->Id() == id)
//				return true;
//
//		return false;
//
//	}
//
//	void ObjectManager::OnBeginUpdate(UpdateEventArgs& e) {
//
//		bool removed = false;
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			if (i->object->IsDestroyed())
//				removed = true;
//
//		if (removed)
//			_removeDestroyedObjects(_objects.begin(), _objects.end());
//
//	}
//	void ObjectManager::OnUpdate(UpdateEventArgs& e) {
//
//		bool removed = false;
//		int last_depth = ObjectBase::MAX_DEPTH;
//
//		for (size_t i = 0; i < _objects.size(); ++i) {
//
//			if (_updateAndCheckObject(i, e))
//				_objects[i].object->OnUpdate(e);
//
//			if (_objects[i].object->IsDestroyed())
//				removed = true;
//			// If the objects are not ordered by depth (descending), set the resort flag.
//			else if (_objects[i].object->Depth() > last_depth)
//				_resort_required = true;
//
//			last_depth = _objects[i].object->Depth();
//
//		}
//
//		if (removed)
//			_removeDestroyedObjects(_objects.begin(), _objects.end());
//
//	}
//	void ObjectManager::OnEndUpdate(UpdateEventArgs& e) {
//
//		bool removed = false;
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			if (i->object->IsDestroyed())
//				removed = true;
//
//		if (removed)
//			_removeDestroyedObjects(_objects.begin(), _objects.end());
//
//	}
//	void ObjectManager::OnDraw(DrawEventArgs& e) {
//
//
//
//	}
//
//	bool ObjectManager::IsSuspendable() const {
//
//		return true;
//
//	}
//
//protected:
//	void AddObject(std::unique_ptr<IObject>&& object) override;
//	void ReceiveContext(const ApplicationContext& context) override;
//	ApplicationContext Context() override;
//
//
//	void ObjectManager::Add(const IObjectPtr& object) {
//
//		assert(static_cast<bool>(object));
//
//		ContextChangedEventArgs args(_context);
//
//		object->OnContextChanged(args);
//
//		//// Trigger all listeners.
//		//for (size_t i = 0; i < _listeners.size(); ++i)
//		//	_listeners[i]->OnInstanceAdded(InstanceAddedEventArgs(object.get()));
//
//		// Add the object to our object collection.
//		_objects.emplace_back(ObjectListItem(object));
//
//		//return _objects.back().object.get();
//
//		//// If there are no objects in the list, just insert the new object.
//		//if (_objects.size() == 0) {
//		//	_objects.push_back(object);
//		//	return;
//		//}
//
//		//// Get the highest and lowest depths in the list.
//		//int lowest_depth = _objects.back()->Depth();
//		//int highest_depth = _objects.front()->Depth();
//
//		//if (object->Depth() <= lowest_depth) 
//		//	// If the object's depth is <= than the lowest depth, insert last.
//		//	_objects.push_back(object);
//
//		//else if (object->Depth() >= highest_depth) 
//		//	// If the objects depth is >= the highest depth, insert first.
//		//	_objects.push_front(object);
//		//
//		//else {
//		//	// Find a proper position for the object according to its depth.
//		//	for (auto it = __objects.begin(); it != __objects.end(); ++it) {
//		//		if ((*it)->Depth() < object->Depth()) {
//		//			__objects.insert(it, object);
//		//			return;
//		//		}
//		//	}
//		//	__objects.push_back(object);
//		//}
//
//	}
//	void ObjectManager::Add(IObject* object) {
//
//		assert(object != nullptr);
//
//		Add(IObjectPtr(object));
//
//	}
//	void ObjectManager::Clear() {
//
//		// Clear all Objects from the Object Manager without calling any events.
//		_objects.clear();
//
//	}
//	void ObjectManager::DestroyAll() {
//
//		// Destroy each Object. They will be removed with the next update.
//		for (auto it = _objects.begin(); it != _objects.end(); ++it)
//			if (!it->object->IsDestroyed())
//				it->object->Destroy();
//
//	}
//	IObject* ObjectManager::Find(ObjectId id) {
//
//		for (size_t i = 0; i < _objects.size(); ++i)
//			if (_objects[i].object->Id() == id)
//				return _objects[i].object.get();
//
//		return nullptr;
//
//	}
//	IObject* ObjectManager::FindNext(ObjectId id) {
//
//
//
//	}
//	size_t ObjectManager::Count() const {
//
//		return _objects.size();
//
//	}
//	size_t ObjectManager::Count(ObjectId id) const {
//
//		size_t count = 0;
//
//		for (size_t i = 0; i < _objects.size(); ++i)
//			if (_objects[i].object->Id() == id)
//				++count;
//
//		return count;
//
//	}
//	bool ObjectManager::Exists(ObjectId id) const {
//
//
//
//	}
//	void ObjectManager::ForEach(const std::function<void(IObjectPtr&)>& func) {
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			func(i->object);
//
//	}
//	void ObjectManager::ForEach(const std::function<void(const IObjectPtr&)>& func) const {
//
//		for (auto i = _objects.begin(); i != _objects.end(); ++i)
//			func(i->object);
//
//	}
//
//	void ObjectManager::OnBeginUpdate(UpdateEventArgs& e) {
//
//
//
//	}
//	void ObjectManager::OnUpdate(UpdateEventArgs& e) {
//
//
//
//	}
//	void ObjectManager::OnEndUpdate(UpdateEventArgs& e) {
//
//		bool removed = false;
//
//		for (size_t i = 0; i < _objects.size(); ++i) {
//
//			if (_updateAndCheckObject(i, e))
//				_objects[i].object->OnEndUpdate(e);
//
//			if (_objects[i].object->IsDestroyed())
//				removed = true;
//
//		}
//
//		if (removed)
//			_removeDestroyedObjects(_objects.begin(), _objects.end());
//
//	}
//	void ObjectManager::OnDraw(DrawEventArgs& e) {
//
//		// If a resort is required, sort all objects by depth before drawing them.
//		// Objects should be sorted such that those with a lower depth are drawn first.
//		if (_resort_required)
//			std::sort(_objects.begin(), _objects.end(),
//				[](const ObjectListItem& lhs, const ObjectListItem& rhs) { return lhs.object->Depth() > rhs.object->Depth(); });
//
//		_resort_required = false;
//
//		// Don't draw any objects that haven't had their create event called yet, or inactive objects.
//
//		for (auto it = _objects.begin(); it != _objects.end(); ++it)
//			if (!it->callOnCreateEvent && it->object->IsActive())
//				it->object->OnDraw(e);
//
//	}
//
//	void ObjectManager::OnContextChanged(ContextChangedEventArgs& e) {
//
//		_context = e.Context();
//
//		// Update the context for all objects.
//		for (auto i = _objects.begin(); i != _objects.end(); ++i) {
//
//			ContextChangedEventArgs args(_context);
//
//			i->object->OnContextChanged(args);
//
//		}
//
//	}
//
//
//	void ObjectManager::_removeDestroyedObjects(object_list_type::iterator begin, object_list_type::iterator end) {
//
//		_objects.erase(std::remove_if(begin, end, [](ObjectListItem& item) {
//
//			bool destroyed = item.object->IsDestroyed();
//
//			if (destroyed && item.callOnDestroyEvent) {
//
//				// The OnDestroy event is triggered here since objects destroyed during their update are removed immediately following the update.
//				// This could also be handled within the update methods themselves, but handling it here results in less code duplication.
//
//				DestroyEventArgs args;
//
//				item.object->OnDestroy(args);
//				item.callOnDestroyEvent = false;
//
//			}
//
//			return destroyed;
//
//		}), _objects.end());
//
//	}
//	bool ObjectManager::_updateAndCheckObject(size_t item_index, UpdateEventArgs& e) {
//
//		// This method takes an index rather than a reference to item because there's no guarantee the vector won't be resized in in OnCreate or OnDestroy.
//		// Passing a reference risks that reference being invalidated after the resize.
//
//		// Call the OnCreate event for the Object if it hasn't been called yet.
//		if (_objects[item_index].callOnCreateEvent) {
//
//			CreateEventArgs args(_context);
//
//			_objects[item_index].object->OnCreate(args);
//			_objects[item_index].callOnCreateEvent = false;
//
//		}
//
//		if (_objects[item_index].object->IsActive())
//			return true;
//
//		// If the Object is not active, its update events should not be called.
//		return false;
//
//	}
//
//}