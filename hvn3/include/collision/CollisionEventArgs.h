#pragma once
#include "events/EventArgs.h"

namespace hvn3 {

	class Object;
	class ICollisionBody;

	class CollisionEventArgs : public System::EventArgs {

	public:
		CollisionEventArgs(Object* other, ICollisionBody* body);

		Object* Object();
		const hvn3::Object* Object() const;
		ICollisionBody* Body();
		ICollisionBody* Body() const;

	private:
		hvn3::Object* _other;
		ICollisionBody* _body;

	};

}