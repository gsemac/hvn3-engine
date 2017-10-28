#include "views/ViewUpdateEventArgs.h"

namespace hvn3 {

	ViewUpdateEventArgs::ViewUpdateEventArgs(float dt, Size<int> room_size) :
		UpdateEventArgs(dt),
		_room_size(room_size) {
	}
	const Size<int> ViewUpdateEventArgs::RoomSize() const {

		return _room_size;

	}

}