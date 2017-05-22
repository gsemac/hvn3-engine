#pragma once
#include "Object.h"
#include <memory>

namespace hvn3 {

	class ICollisionManager;

	class IObjectManager {

	public:
		virtual void InstanceAdd(std::shared_ptr<Object> object) = 0;
		virtual void Clear() = 0;
		virtual std::unique_ptr<ICollisionManager>& CollisionManager() = 0;
		// Finds and returns the first instance with the given id, or null if no such instance exists.
		virtual Object* InstanceFind(ObjectId id) = 0;
		virtual Object* InstanceFindNext(ObjectId id) = 0;
		virtual size_t InstanceCount() const = 0;
		virtual size_t InstanceCount(ObjectId id) const = 0;
		virtual bool InstanceExists(ObjectId id) const = 0;

		virtual void Update(UpdateEventArgs& e) = 0;
		virtual void Draw(DrawEventArgs& e) = 0;

	};

}