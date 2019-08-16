#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/graphics/Graphics.h"

namespace hvn3 {

	class DrawEventArgs :
		public EventArgs {

	public:
		DrawEventArgs(const ApplicationContext& context, Graphics::Graphics& canvas);
		Graphics::Graphics& Graphics();

	private:
		Graphics::Graphics& _graphics;

	};

}