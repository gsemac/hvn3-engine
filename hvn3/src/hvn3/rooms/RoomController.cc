#include "hvn3/rooms/RoomController.h"
#include "hvn3/rooms/IRoom.h"

namespace hvn3 {
	namespace System {

		RoomController::RoomController(IRoom* room) : _room(room) {}

		void RoomController::SetUp() {

			_room->OnCreate();

		}
		bool RoomController::IsReady() const {

			return _room->IsReady();

		}
		void RoomController::Reset() {

			_room->OnReset();

		}

		void RoomController::CallRoomEnterEvent(RoomEnterEventArgs& e) {

			_room->OnEnter(e);

		}
		void RoomController::CallRoomExitEvent(RoomExitEventArgs& e) {

			_room->OnExit(e);

		}

	}
}