#pragma once
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/physics/PhysicsBodyBase.h"
#include "hvn3/physics/PhysicsManagerBase.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsManager :
			public PhysicsManagerBase {

			struct Contact {
				IPhysicsBody* body;
				Vector2d velocity;
			};

		public:
			BasicPhysicsManager();

			void OnUpdate(UpdateEventArgs& e) override;
			void Clear() override;

		protected:
			void OnContextChanged(ContextChangedEventArgs& e) override;

		private:
			std::vector<Contact> _contacts;
			ApplicationContext _context;

		};

	}
}