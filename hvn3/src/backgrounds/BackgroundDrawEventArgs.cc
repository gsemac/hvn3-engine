#include "backgrounds/BackgroundDrawEventArgs.h"
#include "views/View.h"

namespace hvn3 {

	BackgroundDrawEventArgs::BackgroundDrawEventArgs(Drawing::Graphics& graphics, const Size<int>& room_size, const View* current_view) :
		DrawEventArgs(graphics),
		_room_size(room_size),
		_current_view(current_view) {
	}
	const View* BackgroundDrawEventArgs::CurrentView() const {

		return _current_view;

	}
	const Size<int>& BackgroundDrawEventArgs::RoomSize() const {

		return _room_size;

	}

}