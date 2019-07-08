#pragma once

#include "hvn3/backgrounds/Background.h"
#include "hvn3/graphics/Color.h"

#include <cstdint>

namespace hvn3 {

	class IBackgroundManager {

	public:
		using index_type = size_t;
		using size_type = size_t;

		virtual index_type AddBackground(const Background& background) = 0;
		virtual index_type AddBackground(const Background& background, bool foreground) = 0;
		virtual void RemoveBackgroundAt(index_type index) = 0;
		virtual const Background& BackgroundAt(index_type index) const = 0;

		virtual void SetBackgroundColor(const Color& color) = 0;

		virtual size_type Count() const = 0;
		virtual void Clear() = 0;

	};

}