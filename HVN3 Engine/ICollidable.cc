#include "ICollidable.h"
#include "RoomBase.h"
#include "Object.h"

ICollidable::ICollidable(float x, float y) : 
	IPositionable(x, y) {}

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
//::Room& ICollidable::Scene() {
//
//	return *__scene;
//
//}

bool ICollidable::CollidesWith(const ICollidable* other) const {

	return __filter.CheckMatch(other->__filter);

}
void ICollidable::Collide(ICollidable* other) {}
