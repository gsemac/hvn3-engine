#include "hvn3/rooms/RoomTransitionFade.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/utility/Utility.h"

namespace hvn3 {

	RoomTransitionFade::RoomTransitionFade() :
		RoomTransitionFade(Color::Black) {
	}
	RoomTransitionFade::RoomTransitionFade(const Color& fade_to_color) :
		RoomTransitionFade(fade_to_color, false) {
	}
	RoomTransitionFade::RoomTransitionFade(const Color& fade_to_color, bool blocking) :
		_blocking(blocking),
		_alpha(0.0f),
		_fade_to_color(fade_to_color) {
	}

	void RoomTransitionFade::ExitBegin() {

		_alpha = 0.0f;

	}
	bool RoomTransitionFade::ExitStep(UpdateEventArgs& e) {

		_alpha = Math::Min(_alpha + e.Delta(), 1.0);

		return _alpha >= 1.0f;


	}

	bool RoomTransitionFade::EnterStep(UpdateEventArgs& e) {

		_alpha = Math::Max(_alpha - e.Delta(), 0.0);

		return _alpha <= 0.0f;

	}

	void RoomTransitionFade::OnDraw(DrawEventArgs& e) {

		Color draw_color = Color::FromArgbf(_fade_to_color.Rf(), _fade_to_color.Gf(), _fade_to_color.Bf(), _alpha);
		RectangleF region = e.Graphics().Clip();

		e.Graphics().DrawSolidRectangle(0, 0, region.Width(), region.Height(), draw_color);

	}

	bool RoomTransitionFade::Blocking() const {
		return _blocking;
	}

}