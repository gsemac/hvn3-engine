#pragma once

namespace hvn3 {

	namespace Physics {
		class IPhysicsBody;
		class IPhysicsManager;
	}

	namespace System {

		class PhysicsBodyMutator {

		public:
			PhysicsBodyMutator(Physics::IPhysicsBody& body);

			void SetManager(Physics::IPhysicsManager* manager);

		private:
			Physics::IPhysicsBody& _body;


		};

	}
}