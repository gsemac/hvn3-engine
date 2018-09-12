#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Vector2d.h"
#include <cstdint>

namespace hvn3 {

	class ICollider;

	namespace Physics {

		class IPhysicsBody;

		class IPhysicsManager :
			public IUpdatable {
			
		public:
			virtual ~IPhysicsManager() = default;

			// Creates and adds a new body to the system, and returns a pointer to it. The body is owned by the physics manager.
			virtual IPhysicsBody* CreateBody(ICollider* body) = 0;

			virtual const Vector2d& Gravity() const = 0;
			virtual void SetGravity(const Vector2d& value) = 0;
			virtual float PixelsToMetersScale() const = 0;
			virtual void SetPixelsToMetersScale(float value) = 0;
			virtual size_t Count() const = 0;
			virtual void Clear() = 0;


		};

	}
}