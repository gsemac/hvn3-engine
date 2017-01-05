#pragma once
#include "IBroadphase.h"
#include "IUpdatable.h"

class CollisionManager {

public:
	CollisionManager(IBroadphase* broadphase);
	bool TestCollision(ICollidable* a, ICollidable* b) const;
	bool TestCollision(ICollidable* a, float ax, float ay, ICollidable* b, float bx, float by) const;
	IBroadphase& Broadphase();
	void Update();
	
private:
	IBroadphase* __broadphase;
	void ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const;

};