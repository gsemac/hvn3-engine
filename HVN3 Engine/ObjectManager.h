#pragma once
#include "IObjectManager.h"
#include "ICollisionManager.h"
#include <memory>
#include <vector>

namespace hvn3 {

	class ObjectManager : public IObjectManager {

	public:
		ObjectManager(std::unique_ptr<ICollisionManager>& collision_manager);

		// Inherited via IObjectManager
		virtual void AddInstance(std::shared_ptr<Object> object) override;
		virtual void Clear() override;
		virtual std::unique_ptr<ICollisionManager>& CollisionManager() override;
		virtual Object* FindInstance(ObjectId id) override;
		virtual Object* FindNextInstance(ObjectId id) override;
		virtual size_t InstanceCount() const override;
		virtual size_t InstanceCount(ObjectId id) const override;
		virtual bool InstanceExists(ObjectId id) const override;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

	private:
		std::vector<std::shared_ptr<Object>> _objects;
		std::unique_ptr<ICollisionManager> _collision_manager;
		ObjectId _last_found_id;
		size_t _last_found_index;

	};

}