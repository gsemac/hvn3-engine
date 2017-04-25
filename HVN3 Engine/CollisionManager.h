#pragma once
#include <memory>
#include "ICollisionManager.h"
#include "IBroadPhaseCollisionManager.h"

class IBroadPhaseCollisionManager;

class CollisionManager : public ICollisionManager {

public:
	CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager> broadphase);

	void ColliderAdd(ICollidable* collider) override;
	void ColliderRemove(ICollidable* collider) override;
	void Clear() override;

	bool PlaceFree(ICollidable* collider, float x, float y) override;
	void MoveContact(ICollidable* collider, float direction, int max_distance) override;

	void Update(UpdateEventArgs& e) override;
	
private:
	std::unique_ptr<IBroadPhaseCollisionManager> _broadphase;

	void ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const;

};