#pragma once
#include "hvn3/utility/Size.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Display.h"
#include <string>

namespace hvn3 {

	enum class ScalingMode {
		Fixed,
		Full,
		MaintainAspectRatio
	};

	namespace System {

		struct Properties {

		public:
			Properties();

			// Display Properties

			std::string DisplayTitle;
			SizeI DisplaySize;
			bool StartFullscreen;
			hvn3::ScalingMode ScalingMode;
			Color OutsideColor;
			hvn3::DisplayFlags DisplayFlags;

			// Flow Properties

			bool FreezeWhenLostFocus;
			double FrameRate;
			int MaxFrameSkip;
			bool FixedFrameRate;

			// Control Properties

			bool DisplayCursor;
			bool ExitWithEscapeKey;

			// Debug Properties

			bool DebugMode;

		};

	}
}