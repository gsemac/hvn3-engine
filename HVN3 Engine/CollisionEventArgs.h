#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class Object;

	class CollisionEventArgs : public EventArgs {

	public:
		CollisionEventArgs(Object* other);

		Object* Other();
		const Object* Other() const;

	private:
		Object* _other;

	};

}