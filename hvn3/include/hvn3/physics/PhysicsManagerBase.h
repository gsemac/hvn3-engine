#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/physics/IPhysicsManager.h"
#include <vector>

namespace hvn3 {
	namespace Physics {

		class PhysicsManagerBase : public IPhysicsManager {

		public:
			PhysicsManagerBase();

			void AddBody(IPhysicsBody& body) override;
			void RemoveBody(IPhysicsBody& body) override;
			const Vector2d& Gravity() const override;
			void SetGravity(const Vector2d& value) override;
			size_t Count() const override;
			void Clear() override;

			void OnUpdate(UpdateEventArgs& e) override;

		private:
			std::vector<IPhysicsBody*> _bodies;
			std::vector<IPhysicsBody*> _pending_removal;
			Vector2d _gravity;

		};

	}
}