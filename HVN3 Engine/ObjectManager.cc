#include "ObjectManager.h"
#include "ICollisionManager.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"

ObjectManager::ObjectManager(ICollisionManager& collision_manager) {

	_collision_manager = &collision_manager;
	_last_found_id = noone;
	_last_found_index = 0;

}
void ObjectManager::InstanceAdd(std::shared_ptr<Object> object) {

	_objects.push_back(object);

}
void ObjectManager::Clear() {

	_objects.clear();

}
ICollisionManager* ObjectManager::CollisionManager() {

	return _collision_manager;

}
Object* ObjectManager::InstanceFind(ObjectId id) {

	for (size_t i = 0; i < _objects.size(); ++i)
		if (_objects[i]->Id() == id)
			return _objects[i].get();

	return nullptr;

}
Object* ObjectManager::InstanceFindNext(ObjectId id) {
	
	// If the object id we're looking for differs from the last one, reset the found index.
	if (_last_found_id != id)
		_last_found_index = 0;
	_last_found_id = id;

	// Find the next instance, starting at the last found index.
	for (size_t i = _last_found_index + 1; i < _objects.size(); ++i)
		if (_objects[i]->Id() == id)
			return _objects[i].get();

	// If we didn't find any more instances, reset the last found index and return nullptr.
	_last_found_index = 0;
	return nullptr;

}
size_t ObjectManager::InstanceCount() const {

	return _objects.size();

}
size_t ObjectManager::InstanceCount(ObjectId id) const {
	
	size_t count = 0;

	for (size_t i = 0; i < _objects.size(); ++i)
		if (_objects[i]->Id() == id)
			++count;

	return count;

}
bool ObjectManager::InstanceExists(ObjectId id) const {
	
	for (size_t i = 0; i < _objects.size(); ++i)
		if (_objects[i]->Id() == id)
			return true;

	return false;

}
void ObjectManager::Update(UpdateEventArgs& e) {

	// Run the pre-update procedure for all objects.
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
		(*it)->BeginUpdate(e);

	// Run the primary update procedure for all objects.
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
		(*it)->Update(e);

	// Update the Collision Manager.
	_collision_manager->Update(e);

	// Run the post update procedure for all objects.
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
		(*it)->EndUpdate(e);

}
void ObjectManager::Draw(DrawEventArgs& e) {

	// Draw all objects.
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
		(*it)->Draw(e);

}