#pragma once
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/IGameManager.h"
#include "hvn3/events/EventArgs.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"

namespace hvn3 {
	namespace System {

		class GameStateEventArgs : public EventArgs {

		public:
			GameStateEventArgs(IGameManager* manager);

			IGameManager& Game();
			IObjectManager& Objects();
			IRoom& Room();
			ICollisionManager& Collisions();

		private:
			IGameManager* _manager;

		};

	}
}