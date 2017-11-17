#pragma once
#include "events/EventArgs.h"
#include "graphics/Graphics.h"

namespace hvn3 {

	class DrawEventArgs : public System::EventArgs {

	public:
		DrawEventArgs(Graphics::Graphics& graphics);
		Graphics::Graphics& Graphics();

	private:
		Graphics::Graphics& __graphics;

	};

}