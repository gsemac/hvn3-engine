#include "hvn3/events/EventManager.h"
#include "hvn3/objects/ObjectManager.h"
#include "hvn3/utility/Algorithm.h"

#include <utility>

namespace hvn3 {

	// ObjectManager::ObjectInfo

	// Public methods

	ObjectManager::ObjectInfo::ObjectInfo(std::shared_ptr<IObject>&& object) :
		object(std::move(object)) {

		callCreateEvent = true;
		callDestroyEvent = false;
		destroyed = false;

	}
	/*
	ObjectManager::ObjectInfo::ObjectInfo(ObjectInfo&& other) {

		callCreateEvent = other.callCreateEvent;
		callDestroyEvent = other.callDestroyEvent;
		destroyed = other.destroyed;
		object = std::move(other.object);

	}

	ObjectManager::ObjectInfo& ObjectManager::ObjectInfo::operator=(ObjectInfo& other) {

		callCreateEvent = other.callCreateEvent;
		callDestroyEvent = other.callDestroyEvent;
		destroyed = other.destroyed;
		object = other.object;

		return *this;

	}*/

	// ObjectManager

	// Public methods

	ObjectManager::ObjectManager() {

		_update_required = false;

	}

	bool ObjectManager::RemoveObject(ObjectHandle handle) {
		return RemoveObject(handle.operator->().get());
	}
	bool ObjectManager::RemoveObject(IObject* object) {

		return _removeObjectIf([=](const ObjectInfo& x) {
			return !x.destroyed && x.object.get() == object;
		});

	}

	void ObjectManager::OnStart(StartEventArgs& e) {

		e.Context().Get<EventManager>()->SubscribeAll(this);

	}
	void ObjectManager::OnEnd(EndEventArgs& e) {

		e.Context().Get<EventManager>()->UnsubscribeAll(this);

	}

	void ObjectManager::Clear() {

		// Clear all objects without calling OnDestroy.
		_objects.clear();

	}
	void ObjectManager::DestroyAll() {

		for (auto it = _objects.begin(); it != _objects.end(); ++it)
			if (!it->destroyed)
				it->callDestroyEvent = true;

		_update_required = true;

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

	void ObjectManager::OnEvent(UpdateEventArgs& e) {
		_onUpdate(e.Context());
	}

	// Private methods

	void ObjectManager::_addObject(std::shared_ptr<IObject>&& object) {

		_objects.push_back(ObjectInfo(std::move(object)));

		_update_required = true;

	}
	bool ObjectManager::_removeObject(IObject::object_id id) {

		return _removeObjectIf([=](const ObjectInfo& x) {
			return !x.destroyed && x.object->Id() == id;
		});

	}
	bool ObjectManager::_removeObjectIf(const std::function<bool(const ObjectInfo&)>& func) {

		// The OnDestroy event handler will be called during the next update.
		// The object will be permanently removed after.

		auto iter = std::find_if(_objects.begin(), _objects.end(), func);

		if (iter == _objects.end() || iter->destroyed)
			return false;

		iter->callDestroyEvent = true;

		_update_required = true;

		return true;

	}
	void ObjectManager::_onUpdate(ApplicationContext context) {

		if (_update_required) {

			_update_required = false;

			// This is done using indexing instead of iterators since, if event handlers create objects, it could invalidate the iterators.
			// Objects added during this step will have their create events called next update (to avoid an infinite if objects are created in create events).

			size_t size = _objects.size();

			for (size_t i = 0; i < size; ++i) {

				if (_objects[i].callCreateEvent) {

					assert(!_objects[i].callDestroyEvent);
					assert(!_objects[i].destroyed);

					_objects[i].object->OnCreate(IObject::CreateEventArgs(context));
					_objects[i].callCreateEvent = false;

				}

				if (_objects[i].callDestroyEvent) {

					assert(!_objects[i].callCreateEvent);
					assert(!_objects[i].destroyed);

					_objects[i].object->OnDestroy(IObject::DestroyEventArgs(context));
					_objects[i].callDestroyEvent = false;
					_objects[i].destroyed = true;

				}

			}

			// Remove destroyed objects in the range.

			_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [](const ObjectInfo& x) {
				return x.destroyed;
			}), _objects.end());

		}

	}

}