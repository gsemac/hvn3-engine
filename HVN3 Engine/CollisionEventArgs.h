#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class Object;

	class CollisionEventArgs : public EventArgs {

	public:
		CollisionEventArgs(Object* other);

		Object& Other();

	private:
		Object* _other;

	};

}