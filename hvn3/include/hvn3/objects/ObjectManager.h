#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/objects/IObjectManager.h"

#include <memory>
#include <vector>

namespace hvn3 {

	class ObjectManager :
		public ManagerBase<IObjectManager> {

		struct ObjectInfo {

			ObjectInfo(std::shared_ptr<IObject>&& object);
			ObjectInfo(ObjectInfo&& other);

			std::shared_ptr<IObject> object;
			// Set to true when the OnCreate event has been called.
			bool createEventCalled;
			// Set to true when the OnDestroy event has been called.
			bool destroyEventCalled;

		};

	public:
		ObjectManager();

		void OnStart(StartEventArgs& e) override;
		void OnEnd(EndEventArgs& e) override;

		void Clear() override;
		void DestroyAll() override;

		ObjectHandle Find(IObject::object_id id) override;
		bool Exists(IObject::object_id id) const override;

		size_type Count() const = 0;
		size_type Count(IObject::object_id id) const override;

	protected:
		ApplicationContext Context();

	private:
		std::vector<ObjectInfo> _objects;
		ApplicationContext _context;
		size_t _update_start_index;

		void _removeDestroyedObjects(std::vector<ObjectInfo>::iterator begin, std::vector<ObjectInfo>::iterator end);
		// Updates the state of the given object, and calls the OnCreate or OnDestroy event where applicable. Returns true if the object should have its OnUpdate events called; returns false otherwise.
		bool _updateAndCheckObject(size_t item_index, UpdateEventArgs& e);

	};

}