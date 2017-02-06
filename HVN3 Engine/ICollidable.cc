#include "ICollidable.h"
#include "Scene.h"
#include "Object.h"

ICollidable::ICollidable(float x, float y) : 
	IPositionable(x, y) {

	__scene = nullptr;

}

Rectangle ICollidable::AABB() const {

	Rectangle aabb = __mask.AABB();
	aabb.TranslateX(X());
	aabb.TranslateY(Y());

	return aabb;

}
CollisionMask& ICollidable::CollisionMask() {

	return __mask;

}
void ICollidable::SetCollisionMask(const ::CollisionMask& mask) {

	__mask = mask;

}
CollisionFilter& ICollidable::Filter() {

	return __filter;

}
::Scene& ICollidable::Scene() {

	return *__scene;

}

bool ICollidable::CollidesWith(const ICollidable* other) const {

	return __filter.CheckMatch(other->__filter);

}
void ICollidable::Collide(ICollidable* other) {}

bool ICollidable::PlaceFree(float x, float y, bool notme) {

	// If the object is not part of a Scene, it can't collide with anything, so return true.
	if (!__scene)
		return true;

	std::vector<ICollidable*> v;
	__scene->CollisionManager().Broadphase().QueryRegion(AABB(), v, __filter.MaskBits());
	if (v.size() == 0 || (notme && v.size() == 1 && v[0] == this))
		return true;

	for (size_t i = 0; i < v.size(); ++i)
		if (__scene->CollisionManager().TestCollision(this, x, y, v[i], v[i]->X(), v[i]->Y()))
			return false;

	return true;

}
void ICollidable::MoveContact(float direction, int max_distance) {

	for (int i = 0; i < max_distance; ++i) {
		Point new_position = PointInDirection(Point(X(), Y()), direction, 1);
		if (!PlaceFree(new_position.X(), new_position.Y(), true))
			break;
		SetXY(new_position.X(), new_position.Y());
	}

}