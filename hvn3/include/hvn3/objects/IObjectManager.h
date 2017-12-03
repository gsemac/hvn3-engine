#pragma once
#include "hvn3/objects/Object.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IDrawable.h"
#include <memory>

namespace hvn3 {

	class IObjectManagerListener;

	class IObjectManager : public IUpdatable, public IDrawable {

	public:
		virtual void AddInstance(ObjectPtr& object) = 0;
		virtual void AddInstance(Object* object) = 0;
		// Clears all instances without calling their destroy events.
		virtual void ClearAll() = 0;
		// Calls the destroy event for every instance, and clears all instances.
		virtual void DestroyAll() = 0;

		// Finds and returns the first instance with the given id, or null if no such instance exists.
		virtual Object* FindInstance(ObjectId id) = 0;
		virtual Object* FindNextInstance(ObjectId id) = 0;
		virtual size_t InstanceCount() const = 0;
		virtual size_t InstanceCount(ObjectId id) const = 0;
		virtual bool InstanceExists(ObjectId id) const = 0;

		virtual void AddListener(IObjectManagerListener* listener) = 0;
		virtual void RemoveListener(IObjectManagerListener* listener) = 0;
		
	};

}