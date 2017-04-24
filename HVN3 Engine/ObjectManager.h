#pragma once
#include "IObjectManager.h"
#include <memory>
#include <vector>

class ICollisionManager;

class ObjectManager : public IObjectManager {

public:
	ObjectManager(ICollisionManager& collision_manager);

	// Inherited via IObjectManager
	virtual void InstanceAdd(std::shared_ptr<Object> object) override;
	virtual void Clear() override;
	virtual ICollisionManager* CollisionManager() override;
	virtual Object* InstanceFind(ObjectId id) override;
	virtual Object* InstanceFindNext(ObjectId id) override;
	virtual size_t InstanceCount() const override;
	virtual size_t InstanceCount(ObjectId id) const override;
	virtual bool InstanceExists(ObjectId id) const override;

	virtual void Update(UpdateEventArgs& e) override;
	virtual void Draw(DrawEventArgs& e) override;

private:
	std::vector<std::shared_ptr<Object>> _objects;
	ICollisionManager* _collision_manager;
	ObjectId _last_found_id;
	size_t _last_found_index;

};