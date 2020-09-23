#pragma once

#include "hvn3/objects/IObject.h"
#include "hvn3/objects/ObjectDefs.h"
#include "hvn3/objects/ObjectHandle.h"

#include <functional>
#include <memory>
#include <utility>

namespace hvn3 {

	class IObjectManager {

	public:
		typedef size_t size_type;

		virtual ~IObjectManager() = default;

		// Clears all objects without calling event handlers.
		virtual void Clear() = 0;
		// Destroys all objects and calls the appropriate event handler(s).
		virtual void DestroyAll() = 0;

		// Returns a handle to the first object with the given ID.
		virtual ObjectHandle Find(IObject::object_id id) = 0;
		// Returns true if there are any objects with the given ID.
		virtual bool Exists(IObject::object_id id) const = 0;

		virtual size_type Count() const = 0;
		virtual size_type Count(IObject::object_id id) const = 0;

	};

}