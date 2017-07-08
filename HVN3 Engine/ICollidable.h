#pragma once
#include "CollisionEventArgs.h"

namespace hvn3 {

	class Collider;

	class ICollidable {

	public:
		virtual void OnCollision(CollisionEventArgs& e) = 0;

	};

}