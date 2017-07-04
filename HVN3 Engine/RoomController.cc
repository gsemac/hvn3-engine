#include "RoomController.h"
#include "RoomBase.h"

namespace hvn3 {

	RoomController::RoomController(RoomBase& room) : _room(&room) {}

	void RoomController::SetUp() {

		_room->OnSetUp();
		SetSetUp(true);

	}
	bool RoomController::IsSetUp() const {

		return _room->_set_up;

	}
	void RoomController::SetSetUp(bool value) {

		_room->_set_up = value;

	}
	void RoomController::Reset() {

		_room->OnReset();
		SetSetUp(false);

	}

	void RoomController::CallRoomEnterEvent(RoomEnterEventArgs& e) {

		_room->OnRoomEnter(e);

	}
	void RoomController::CallRoomExitEvent(RoomExitEventArgs& e) {

		_room->OnRoomExit(e);

	}

}