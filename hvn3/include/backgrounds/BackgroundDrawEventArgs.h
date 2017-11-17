#pragma once
#include "core/DrawEventArgs.h"
#include "utility/Size.h"

namespace hvn3 {

	class View;

	class BackgroundDrawEventArgs : public DrawEventArgs {

	public:
		BackgroundDrawEventArgs(Graphics::Graphics& graphics, const Size<int>& room_size, const View* current_view);
		const View* CurrentView() const;
		const Size<int>& RoomSize() const;

	private:
		const View* _current_view;
		Size<int> _room_size;

	};

}