#pragma once
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class View;

	class BackgroundDrawEventArgs : public DrawEventArgs {

	public:
		BackgroundDrawEventArgs(Graphics::Graphics& graphics, const SizeI& room_size, const View* current_view, bool draw_foregrounds);
		const View* CurrentView() const;
		const SizeI& RoomSize() const;
		bool DrawForegrounds() const;

	private:
		const View* _current_view;
		SizeI _room_size;
		bool _draw_foregrounds;

	};

}