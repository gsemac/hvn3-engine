#pragma once

#include "graphics/igraphics.h"

namespace hvn3::core {

	class IDisplay {

	public:
		virtual ~IDisplay() = default;

		virtual graphics::IGraphics& GetCanvas() = 0;

		virtual void Refresh() = 0;

		virtual explicit operator bool() const = 0;

	};

}