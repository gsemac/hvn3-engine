#include "hvn3/collision/CollisionEventArgs.h"

namespace hvn3 {

	CollisionEventArgs::CollisionEventArgs(ICollider* other) :
		_other_body(other) {
	}

	ICollider* CollisionEventArgs::Body() {
		return _other_body;
	}

}