#pragma once

#include <limits>

namespace hvn3 {

	class DrawEventArgs;

	class IDrawable {

	public:
		typedef int depth_type;

		static constexpr depth_type MIN_DEPTH = std::numeric_limits<depth_type>::min();
		static constexpr depth_type MAX_DEPTH = std::numeric_limits<depth_type>::max();

		virtual void OnDraw(DrawEventArgs& e) = 0;

		virtual depth_type Depth() const = 0;
		virtual void SetDepth(depth_type value) = 0;

	};

}