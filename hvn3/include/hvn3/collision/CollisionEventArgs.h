#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class IObject;
	class ICollisionBody;

	class CollisionEventArgs : public System::EventArgs {

	public:
		CollisionEventArgs(IObject* other, ICollisionBody* other_body);

		ICollisionBody* Body() const;
		const IObject* Object() const;
		IObject* GetObject();
		ICollisionBody* GetBody();
		
	private:
		IObject* _other;
		ICollisionBody* _other_body;

	};

}