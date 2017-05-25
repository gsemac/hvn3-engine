#include "RoomTransitionFade.h"
#include "DrawEventArgs.h"
#include "Utility.h"

namespace hvn3 {

	RoomTransitionFade::RoomTransitionFade() :
		RoomTransitionFade(Color::Black) {
	}
	RoomTransitionFade::RoomTransitionFade(const Color& fade_to_color) :
		RoomTransitionFade(fade_to_color, false) {
	}
	RoomTransitionFade::RoomTransitionFade(const Color& fade_to_color, bool allow_room_update) :
		_allow_update(allow_room_update),
		_alpha(0.0f),
		_fade_to_color(fade_to_color) {
	}

	void RoomTransitionFade::ExitBegin() {

		_alpha = 0.0f;

	}
	bool RoomTransitionFade::ExitStep(UpdateEventArgs& e) {

		_alpha = Min(_alpha + e.Delta(), 1.0f);

		return _alpha >= 1.0f;


	}

	bool RoomTransitionFade::EnterStep(UpdateEventArgs& e) {

		_alpha = Max(_alpha - e.Delta(), 0.0f);

		return _alpha <= 0.0f;

	}

	void RoomTransitionFade::Draw(DrawEventArgs& e) {

		Color draw_color(_fade_to_color.R(), _fade_to_color.G(), _fade_to_color.B(), _alpha);
		Rectangle region = e.Graphics().Clip();

		e.Graphics().DrawFilledRectangle(0, 0, region.Width(), region.Height(), draw_color);

	}

	bool RoomTransitionFade::AllowRoomUpdate() const {

		return _allow_update;

	}

}