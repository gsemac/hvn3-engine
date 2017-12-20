#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Vector2d.h"
#include <cstdint>

namespace hvn3 {
	namespace Physics {

		class IPhysicsBody;

		class IPhysicsManager : public IUpdatable {
			
		public:
			virtual ~IPhysicsManager() = default;

			// Adds a new body to the physics manager.
			virtual void AddBody(IPhysicsBody& body) = 0;
			// Marks a body for removal from the physics manager.
			virtual void RemoveBody(IPhysicsBody& body) = 0;
			virtual const Vector2d& Gravity() const = 0;
			virtual void SetGravity(const Vector2d& value) = 0;
			virtual float PixelsToMetersScale() const = 0;
			virtual void SetPixelsToMetersScale(float value) = 0;
			virtual size_t Count() const = 0;
			virtual void Clear() = 0;


		};

	}
}