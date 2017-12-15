#include "hvn3/core/GameStateEventArgs.h"
#include "hvn3/core/IGameManager.h"
#include "hvn3/rooms/RoomManager.h"

namespace hvn3 {
	namespace System {

		GameStateEventArgs::GameStateEventArgs(IGameManager* manager) :
			_manager(manager) {
		}

		IGameManager& GameStateEventArgs::Game() {

			return *_manager;

		}
		IObjectManager& GameStateEventArgs::Objects() {

			return *Room().Objects();

		}
		IRoom& GameStateEventArgs::Room() {

			return *Game().Rooms().CurrentRoom();

		}
		ICollisionManager& GameStateEventArgs::Collisions() {

			return *Room().Collisions();

		}

	}
}