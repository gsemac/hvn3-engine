#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/graphics/Graphics.h" 
#include "hvn3/math/Rectangle.h"
#include "hvn3/rooms/SceneTransitionFade.h"

namespace hvn3 {

	SceneTransitionFade::SceneTransitionFade() :
		SceneTransitionFade(Color::Black) {
	}
	SceneTransitionFade::SceneTransitionFade(const Color& fade_to_color) :
		SceneTransitionFade(fade_to_color, false) {
	}
	SceneTransitionFade::SceneTransitionFade(const Color& fade_to_color, bool blocking) :
		_blocking(blocking),
		_alpha(0.0f),
		_fade_to_color(fade_to_color) {
	}
	void SceneTransitionFade::OnExitBegin() {

		_alpha = 0.0f;

	}
	bool SceneTransitionFade::OnExitStep(ExitStepEventArgs& e) {

		_alpha = static_cast<float>(Math::Min(_alpha + e.Delta(), 1.0));

		return _alpha >= 1.0f;

	}
	bool SceneTransitionFade::OnEnterStep(EnterStepEventArgs& e) {

		_alpha = static_cast<float>(Math::Max(_alpha - e.Delta(), 0.0));

		return _alpha <= 0.0f;

	}
	void SceneTransitionFade::OnDraw(DrawEventArgs& e) {

		Color draw_color = Color::FromArgbf(_fade_to_color.Rf(), _fade_to_color.Gf(), _fade_to_color.Bf(), _alpha);
		
		e.Graphics().Fill(draw_color);

	}
	bool SceneTransitionFade::Blocking() const {
		return _blocking;
	}

}