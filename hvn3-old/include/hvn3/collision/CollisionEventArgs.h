#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class ICollider;

	class CollisionEventArgs : 
		public EventArgs {

	public:
		CollisionEventArgs(ICollider* other);

		ICollider* Body();
		
	private:
		ICollider* _other_body;

	};

}