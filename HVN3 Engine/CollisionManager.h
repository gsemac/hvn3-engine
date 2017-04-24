#pragma once
#include "IBroadPhaseCollisionManager.h"
#include "IUpdatable.h"

class IBroadPhaseCollisionManager;
class INarrowPhaseCollisionManager;

class CollisionManager {

public:
	CollisionManager(IBroadPhaseCollisionManager* broadphase);
	bool TestCollision(ICollidable* a, ICollidable* b) const;
	bool TestCollision(ICollidable* a, float ax, float ay, ICollidable* b, float bx, float by) const;
	IBroadPhaseCollisionManager& Broadphase();
	void Update();
	
private:
	IBroadPhaseCollisionManager* __broadphase;
	void ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const;

};