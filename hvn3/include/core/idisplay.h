#pragma once

#include "graphics/igraphics.h"

namespace hvn3::core {

	class IDisplay {

	public:
		virtual ~IDisplay() = default;

		virtual graphics::IGraphics& GetCanvas() = 0;

	};

}