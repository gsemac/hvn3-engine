#pragma once
#include "EventArgs.h"
#include "Graphics.h"

namespace hvn3 {

	class DrawEventArgs : public EventArgs {

	public:
		DrawEventArgs(Drawing::Graphics& graphics);
		Drawing::Graphics& Graphics();

	private:
		Drawing::Graphics& __graphics;

	};

}