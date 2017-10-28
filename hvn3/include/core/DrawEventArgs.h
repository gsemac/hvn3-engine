#pragma once
#include "events/EventArgs.h"
#include "graphics/Graphics.h"

namespace hvn3 {

	class DrawEventArgs : public System::EventArgs {

	public:
		DrawEventArgs(Drawing::Graphics& graphics);
		Drawing::Graphics& Graphics();

	private:
		Drawing::Graphics& __graphics;

	};

}