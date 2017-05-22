#include "ViewUpdateEventArgs.h"

namespace hvn3 {

	ViewUpdateEventArgs::ViewUpdateEventArgs(float dt, Size room_size) :
		UpdateEventArgs(dt),
		_room_size(room_size) {
	}
	const Size ViewUpdateEventArgs::RoomSize() const {

		return _room_size;

	}

}