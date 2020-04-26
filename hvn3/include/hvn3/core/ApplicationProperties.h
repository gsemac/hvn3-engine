#pragma once
#include "hvn3/core/CoreDefs.h"
#include "hvn3/utility/Size.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/io/window.h"
#include <string>

namespace hvn3 {

	class ApplicationProperties {

	public:
		ApplicationProperties();

		// Display Properties

		std::string ApplicationName;
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