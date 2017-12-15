#include "hvn3/core/DrawEventArgs.h"

namespace hvn3 {

	DrawEventArgs::DrawEventArgs(Graphics::Graphics& graphics) :
		DrawEventArgs(graphics, nullptr) {
	}
	DrawEventArgs::DrawEventArgs(Graphics::Graphics& graphics, IGameManager* manager) :
		GameStateEventArgs(manager),
		_graphics(graphics) {
	}
	Graphics::Graphics& DrawEventArgs::Graphics() {

		return _graphics;

	}

}