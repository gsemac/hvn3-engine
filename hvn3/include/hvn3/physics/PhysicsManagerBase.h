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
			float PixelsToMetersScale() const override;
			void SetPixelsToMetersScale(float value) override;
			size_t Count() const override;
			void Clear() override;

			void OnUpdate(UpdateEventArgs& e) override;

		protected:
			typedef std::vector<IPhysicsBody*> bodies_list_type;

			bodies_list_type& Bodies();
			const bodies_list_type& Bodies() const;

		private:
			bodies_list_type _bodies;
			bodies_list_type _pending_removal;
			Vector2d _gravity;
			float _pixels_to_meters_scale;

		};

	}
}