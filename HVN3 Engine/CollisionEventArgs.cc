#include "CollisionEventArgs.h"
#include "Object.h"

namespace hvn3 {

	CollisionEventArgs::CollisionEventArgs(Object* other) :
		_other(other) {}

	Object& CollisionEventArgs::Other() {

		return *_other;

	}

}