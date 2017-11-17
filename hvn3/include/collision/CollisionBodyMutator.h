#pragma once

namespace hvn3 {

	class ICollisionBody;
	class IBroadPhase;
	class INarrowPhase;

	namespace System {

		class CollisionBodyMutator {

		public:
			CollisionBodyMutator(ICollisionBody& body);

			void SetPhasePair(IBroadPhase* broad_phase, INarrowPhase* narrow_phase);
			
		private:
			ICollisionBody& _body;


		};

	}
}