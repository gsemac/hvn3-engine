#pragma once

#include "graphics/igraphics.h"

#include <cstdint>

namespace hvn3::io {

	class IDisplay {

	public:
		virtual ~IDisplay() = default;

		virtual uint32_t Id() const = 0;

		virtual graphics::IGraphics& GetCanvas() = 0;

		virtual void Refresh() = 0;

		virtual explicit operator bool() const = 0;

	};

}