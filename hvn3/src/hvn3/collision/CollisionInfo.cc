#include "hvn3/collision/CollisionInfo.h"

namespace hvn3 {

	CollisionInfo::CollisionInfo(bool colliding) :
		_colliding(colliding),
		_penetration_depth(0.0f) {
	}
	CollisionInfo::CollisionInfo(const Vector2d& normal, float penetrationDepth) :
		_colliding(true),
		_collision_normal(normal),
		_penetration_depth(penetrationDepth) {
	}

	const Vector2d& CollisionInfo::Normal() const {
		return _collision_normal;
	}
	float CollisionInfo::PenetrationDepth() const {
		return _penetration_depth;
	}

	CollisionInfo::operator bool() const {
		return _colliding;
	}

}