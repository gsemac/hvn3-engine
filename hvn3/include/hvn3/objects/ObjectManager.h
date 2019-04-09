#pragma once

#include "hvn3/core/IApplicationContextReceiver.h"
#include "hvn3/objects/IObjectManager.h"

#include <memory>
#include <vector>

namespace hvn3 {

	class ObjectManager :
		public IObjectManager {

		struct ObjectInfo {

			ObjectInfo(std::unique_ptr<IObject>&& object);
			ObjectInfo(ObjectInfo&& other);

			std::unique_ptr<IObject> object;
			// This is set to true when the OnCreate event has been called.
			bool createEventCalled;
			// This is set to true when the OnDestroy event has been called.
			bool destroyEventCalled;

		};

	public:
		ObjectManager();

		// Clears all instances without calling their destroy events.
		void Clear() override;
		// Calls the destroy event for every instance, and clears all instances.
		void DestroyAll() override;

		// Finds and returns the first instance with the given id, or null if no such instance exists.
		virtual IObject* Find(IObject::object_id id) override;
		virtual IObject* FindNext(IObject::object_id id) override;

		size_t Count() const = 0;
		size_t Count(IObject::object_id id) const override;
		bool Exists(IObject::object_id id) const override;

		void OnBeginUpdate(UpdateEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;
		void OnEndUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

		bool IsSuspendable() const override;

	protected:
		void AddObject(std::unique_ptr<IObject>&& object) override;
		void ReceiveContext(const ApplicationContext& context) override;
		ApplicationContext Context() override;

	private:
		std::vector<ObjectInfo> _objects;
		IObject::object_id _last_found_id;
		size_t _last_found_index;
		ApplicationContext _context;
		bool _resort_required;

		void _removeDestroyedObjects(std::vector<ObjectInfo>::iterator begin, std::vector<ObjectInfo>::iterator end);
		// Updates the state of the given object, and calls the OnCreate or OnDestroy event where applicable. Returns true if the object should have its OnUpdate events called; returns false otherwise.
		bool _updateAndCheckObject(size_t item_index, UpdateEventArgs& e);

	};

}