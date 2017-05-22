#pragma once
#include "DrawEventArgs.h"
#include "Size.h"

namespace hvn3 {

	class View;

	class BackgroundDrawEventArgs : public DrawEventArgs {

	public:
		BackgroundDrawEventArgs(Drawing::Graphics& graphics, Size room_size, View* current_view);
		const View* CurrentView() const;
		const Size& RoomSize() const;

	private:
		View* _current_view;
		Size _room_size;

	};

}