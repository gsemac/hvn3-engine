#include "Properties.h"

namespace hvn3 {

	Properties::Properties() :
		DisplaySize(640, 480) {

		DisplayTitle = "HVN3 Engine";
		StartFullscreen = false;
		ScalingMode = hvn3::ScalingMode::MaintainAspectRatio;
		OutsideColor = Color::Black;
		DisplayFlags = hvn3::DisplayFlags::None;

		FreezeWhenLostFocus = true;
		Fps = 60.0f;
		MaxFrameSkip = 0;
		FixedFrameRate = true;

		DisplayCursor = true;
		ExitWithEscapeKey = true;

		DebugMode = false;

	}

}