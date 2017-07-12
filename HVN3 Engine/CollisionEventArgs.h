#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class Object;

	namespace Collision {
		class ICollisionBody;
	}

	class CollisionEventArgs : public EventArgs {

	public:
		CollisionEventArgs(Object* other, Collision::ICollisionBody* body);

		Object* Object();
		const hvn3::Object* Object() const;
		Collision::ICollisionBody* Body();
		Collision::ICollisionBody* Body() const;

	private:
		hvn3::Object* _other;
		Collision::ICollisionBody* _body;

	};

}