#include "hvn3/core/GameEventArgs.h"
#include "hvn3/core/IGameManager.h"
#include "hvn3/rooms/RoomManager.h"

namespace hvn3 {
	namespace System {

		GameEventArgs::GameEventArgs(IGameManager& game_manager) :
			_game_manager(&game_manager) {
		}

		IGameManager& GameEventArgs::Game() {

			return *_game_manager;

		}
		IObjectManager& GameEventArgs::Objects() {

			return *_game_manager->Rooms().CurrentRoom()->Objects();

		}
		IRoom& GameEventArgs::Room() {

			return *_game_manager->Rooms().CurrentRoom();

		}
		ICollisionManager& GameEventArgs::Collisions() {

			return *_game_manager->Rooms().CurrentRoom()->Collisions();

		}

	}
}