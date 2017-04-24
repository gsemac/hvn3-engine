#pragma once
#include "ICollisionManager.h"
#include "IBroadPhaseCollisionManager.h"

class IBroadPhaseCollisionManager;
class INarrowPhaseCollisionManager;

class CollisionManager : public ICollisionManager {

public:
	CollisionManager(IBroadPhaseCollisionManager* broadphase);
	bool TestCollision(ICollidable* a, ICollidable* b) const;
	bool TestCollision(ICollidable* a, float ax, float ay, ICollidable* b, float bx, float by) const;
	IBroadPhaseCollisionManager& Broadphase();
	void Update(UpdateEventArgs& e) override;
	
private:
	IBroadPhaseCollisionManager* __broadphase;
	void ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const;

};