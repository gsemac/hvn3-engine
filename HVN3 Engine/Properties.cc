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
		FPS = 60.0f;
		MaxFrameSkip = 0;

		DisplayCursor = true;
		ExitWithEscapeKey = true;

		DebugMode = false;

	}

}