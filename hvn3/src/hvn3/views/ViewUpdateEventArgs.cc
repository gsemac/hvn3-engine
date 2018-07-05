#include "hvn3/views/ViewUpdateEventArgs.h"

namespace hvn3 {

	ViewUpdateEventArgs::ViewUpdateEventArgs(double dt, Size<int> room_size) :
		UpdateEventArgs(dt),
		_room_size(room_size) {
	}
	const Size<int> ViewUpdateEventArgs::RoomSize() const {

		return _room_size;

	}

}