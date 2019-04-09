#pragma once

#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/ObjectDefs.h"

#include <functional>
#include <memory>
#include <utility>

namespace hvn3 {

	class IObjectManager :
		public IManager,
		public IUpdatable,
		public IDrawable {

	public:
		virtual ~IObjectManager() = default;

		// Clears all instances without calling their destroy events.
		virtual void Clear() = 0;
		// Calls the destroy event for every instance, and clears all instances.
		virtual void DestroyAll() = 0;

		// Finds and returns the first instance with the given id, or null if no such instance exists.
		virtual IObject* Find(IObject::object_id id) = 0;
		virtual IObject* FindNext(IObject::object_id id) = 0;

		virtual size_t Count() const = 0;
		virtual size_t Count(IObject::object_id id) const = 0;
		virtual bool Exists(IObject::object_id id) const = 0;

	protected:
		// Adds a new instance of an object to the manager. The manager assumes ownership.
		virtual void AddObject(std::unique_ptr<IObject>&& object) = 0;

	};

}