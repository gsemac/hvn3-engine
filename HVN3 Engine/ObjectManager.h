#pragma once
#include <memory>

class ICollisionManager;

class ObjectManager {

public:
	ObjectManager(ICollisionManager* collision_manager);

private:
	ICollisionManager* _collision_manager;

};