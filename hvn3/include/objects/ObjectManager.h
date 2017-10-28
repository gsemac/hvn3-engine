#pragma once
#include "IObjectManager.h"
#include "collision/ICollisionManager.h"
#include <memory>
#include <vector>

namespace hvn3 {

	class ObjectManager : public IObjectManager {

	public:
		ObjectManager();

		virtual void AddInstance(ObjectPtr& object) override;
		virtual void AddInstance(Object* object) override;
		virtual void ClearAll() override;
		virtual void DestroyAll() override;

		virtual Object* FindInstance(ObjectId id) override;
		virtual Object* FindNextInstance(ObjectId id) override;
		virtual size_t InstanceCount() const override;
		virtual size_t InstanceCount(ObjectId id) const override;
		virtual bool InstanceExists(ObjectId id) const override;

		virtual void OnBeginUpdate(UpdateEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnEndUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		virtual void AddListener(IObjectManagerListener* listener) override;
		virtual void RemoveListener(IObjectManagerListener* listener) override;

	private:
		std::vector<ObjectPtr> _objects;
		std::vector<IObjectManagerListener*> _listeners;
		ObjectId _last_found_id;
		size_t _last_found_index;

		void _removeDestroyedObjects(std::vector<ObjectPtr>::iterator begin, std::vector<ObjectPtr>::iterator end);

	};

}