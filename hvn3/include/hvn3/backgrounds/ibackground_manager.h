#pragma once

#include "hvn3/backgrounds/background.h"
#include "hvn3/graphics/Color.h"

#include <cstdint>

namespace hvn3::backgrounds {

	class IBackgroundManager {

	public:
		using size_type = std::size_t;

		virtual Background& AddBackground(const Background& background) = 0;

		virtual void SetBackgroundColor(const Color& color) = 0;

		virtual size_type Count() const = 0;

		virtual void Clear() = 0;

	};

}