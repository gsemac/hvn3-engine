#pragma once
#include "hvn3/collision/CollisionEventArgs.h"

namespace hvn3 {
	class ICollidable {

	public:
		virtual void OnCollision(CollisionEventArgs& e) = 0;

	};
}