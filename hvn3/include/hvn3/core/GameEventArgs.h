#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {
	
	class ICollisionManager;
	class IGameManager;
	class IObjectManager;
	class IRoom;

	namespace System {

		class GameEventArgs : public EventArgs {

		public:
			GameEventArgs(IGameManager& game_manager);

			IGameManager& Game();
			IObjectManager& Objects();
			IRoom& Room();
			ICollisionManager& Collisions();

		private:
			IGameManager* _game_manager;

		};

	}
}