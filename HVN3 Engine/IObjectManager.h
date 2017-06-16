#pragma once
#include "Object.h"
#include <memory>

namespace hvn3 {

	class ICollisionManager;

	class IObjectManager {

	public:
		virtual void AddInstance(ObjectPtr object) = 0;
		virtual void Clear() = 0;
		virtual std::unique_ptr<ICollisionManager>& CollisionManager() = 0;
		// Finds and returns the first instance with the given id, or null if no such instance exists.
		virtual Object* FindInstance(ObjectId id) = 0;
		virtual Object* FindNextInstance(ObjectId id) = 0;
		virtual size_t InstanceCount() const = 0;
		virtual size_t InstanceCount(ObjectId id) const = 0;
		virtual bool InstanceExists(ObjectId id) const = 0;

		virtual void OnUpdate(UpdateEventArgs& e) = 0;
		virtual void OnDraw(DrawEventArgs& e) = 0;

	};

}