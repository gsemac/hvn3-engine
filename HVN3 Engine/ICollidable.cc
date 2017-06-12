#include "ICollidable.h"
#include "RoomBase.h"
#include "Object.h"

namespace hvn3 {

	ICollidable::ICollidable(float x, float y) :
		IPositionable2d<float>(x, y) {
	}

	Rectangle<float> ICollidable::AABB() const {

		Rectangle<float> aabb = __mask.AABB();
		aabb.Translate(X(), Y());

		return aabb;

	}
	CollisionMask& ICollidable::CollisionMask() {

		return __mask;

	}
	void ICollidable::SetCollisionMask(const hvn3::CollisionMask& mask) {

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

}