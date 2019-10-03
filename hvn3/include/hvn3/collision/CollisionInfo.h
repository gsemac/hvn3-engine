#pragma once

#include "hvn3/math/Vector2d.h"

namespace hvn3 {

	class CollisionInfo {

	public:
		CollisionInfo(bool colliding);
		CollisionInfo(const Vector2d& normal, float penetrationDepth);

		const Vector2d& Normal() const;
		float PenetrationDepth() const;

		explicit operator bool() const;

	private:
		bool _colliding;
		Vector2d _collision_normal;
		float _penetration_depth;

	};

}