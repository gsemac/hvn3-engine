#pragma once
#include "hvn3/physics/PhysicsManagerBase.h"
#include <unordered_map>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsManager : public PhysicsManagerBase {

		public:
			void AddBody(IPhysicsBody& body) override;
			void RemoveBody(IPhysicsBody& body) override;
			
			void OnUpdate(UpdateEventArgs& e) override;

		private:
			std::unordered_map<ICollisionBody*, IPhysicsBody*> _body_lookup_table;

			IPhysicsBody* _lookupBody(ICollisionBody* key);

		};

	}
}