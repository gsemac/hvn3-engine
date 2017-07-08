#pragma once
#include "IPhysicsBody.h"
#include "IUpdatable.h"

namespace hvn3 {
	namespace Physics {

		template <typename key_type>
		class IPhysicsManager : public IUpdatable {
			
		public:
			typedef key_type key_type;

			virtual IPhysicsBody* GetBody(key_type key) = 0;
			virtual const IPhysicsBody* GetBody(key_type key) const = 0;
			virtual IPhysicsBody* CreateBody(key_type key) = 0;

			virtual const Vector2d& Gravity() const = 0;
			virtual void SetGravity(const Vector2d& value) = 0;

		};

	}
}