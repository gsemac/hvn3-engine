#pragma once
#include "hvn3/physics/PhysicsManagerBase.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsManager : public PhysicsManagerBase {

			struct Contact {
				IPhysicsBody* body;
				Vector2d velocity;
			};

		public:
			void AddBody(IPhysicsBody& body) override;
			void RemoveBody(IPhysicsBody& body) override;
			
			void OnUpdate(UpdateEventArgs& e) override;

		private:
			std::unordered_map<ICollisionBody*, IPhysicsBody*> _body_lookup_table;
			std::vector<Contact> _contacts;

			IPhysicsBody* _lookupBody(ICollisionBody* key);

		};

	}
}