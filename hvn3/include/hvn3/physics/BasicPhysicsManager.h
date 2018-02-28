#pragma once
#include "hvn3/physics/PhysicsBodyBase.h"
#include "hvn3/physics/PhysicsManagerBase.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsManager : public PhysicsManagerBase<PhysicsBodyBase> {

			struct Contact {
				IPhysicsBody* body;
				Vector2d velocity;
			};

		public:
			IPhysicsBody* CreateBody(ICollisionBody* body) override;
			void OnUpdate(UpdateEventArgs& e) override;
			void Clear() override;

		private:
			std::unordered_map<ICollisionBody*, IPhysicsBody*> _body_lookup_table;
			std::vector<Contact> _contacts;

			IPhysicsBody* _lookupBody(ICollisionBody* key);

		};

	}
}