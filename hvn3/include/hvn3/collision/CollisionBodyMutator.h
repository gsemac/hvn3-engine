#pragma once

namespace hvn3 {

	class ICollisionBody;
	class ICollisionManager;

	namespace System {

		class CollisionBodyMutator {

		public:
			CollisionBodyMutator(ICollisionBody* body);

			void SetManager(ICollisionManager* manager);
			
		private:
			ICollisionBody* _body;


		};

	}
}