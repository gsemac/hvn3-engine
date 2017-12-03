#include "hvn3/rooms/RoomController.h"
#include "hvn3/rooms/IRoom.h"

namespace hvn3 {
	namespace System {

		RoomController::RoomController(IRoom* room) : _room(room) {}

		void RoomController::SetUp() {

			_room->OnSetUp();

		}
		bool RoomController::IsSetUp() const {

			return _room->IsSetUp();

		}
		void RoomController::Reset() {

			_room->OnReset();

		}

		void RoomController::CallRoomEnterEvent(RoomEnterEventArgs& e) {

			_room->OnRoomEnter(e);

		}
		void RoomController::CallRoomExitEvent(RoomExitEventArgs& e) {

			_room->OnRoomExit(e);

		}

	}
}