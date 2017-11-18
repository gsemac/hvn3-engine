#pragma once
#include "core/UpdateEventArgs.h"
#include "backgrounds/BackgroundDrawEventArgs.h"
#include "backgrounds/Background.h"
#include <cstdint>

namespace hvn3 {

	class IBackgroundManager {

	public:
		virtual size_t Add(const Background& background) = 0;
		virtual size_t Add(const Background& background, bool foreground) = 0;
		virtual void Remove(size_t index) = 0;
		virtual const Background& At(size_t index) const = 0;
		virtual size_t Count() const = 0;
		virtual void Clear() = 0;

		virtual void Update(UpdateEventArgs& e) = 0;
		virtual void Draw(BackgroundDrawEventArgs& e) = 0;

	};

}