#pragma once
#include "hvn3/collision/CollisionEventArgs.h"

namespace hvn3 {
	class ICollidable {

	public:
		virtual void OnCollide(CollisionEventArgs& e) = 0;

	};
}