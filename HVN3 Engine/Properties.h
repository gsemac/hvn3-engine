#pragma once
#include "Size.h"
#include "Color.h"
#include "Display.h"
#include <string>

namespace hvn3 {

	enum class ScalingMode {
		Fixed,
		Full,
		MaintainAspectRatio
	};

	struct Properties {

	public:
		Properties();

		// Display Properties

		std::string DisplayTitle;
		Size DisplaySize;
		bool StartFullscreen;
		hvn3::ScalingMode ScalingMode;
		Color OutsideColor;
		hvn3::DisplayFlags DisplayFlags;

		// Flow Properties

		bool FreezeWhenLostFocus;
		float FPS;
		int MaxFrameSkip;

		// Control Properties

		bool DisplayCursor;
		bool ExitWithEscapeKey;

		// Debug Properties

		bool DebugMode;

	};

}