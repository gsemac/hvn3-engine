#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class ICollisionManager;
	class IGameManager;
	class IObjectManager;
	class IRoom;

	namespace Physics {
		class IPhysicsManager;
	}

	namespace System {
		
		class GameStateEventArgs : public EventArgs {

		public:
			GameStateEventArgs(IGameManager* manager);

			IGameManager& Game();
			IObjectManager& Objects();
			IRoom& Room();
			ICollisionManager& Collisions();
			Physics::IPhysicsManager& Physics();

		private:
			IGameManager* _manager;

		};

	}
}