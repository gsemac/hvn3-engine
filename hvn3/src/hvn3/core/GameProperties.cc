#include "hvn3/core/GameProperties.h"

namespace hvn3 {

	GameProperties::GameProperties() :
		DisplaySize(640, 480) {

		DisplayTitle = "HVN3 Engine";
		StartFullscreen = false;
		ScalingMode = hvn3::ScalingMode::MaintainAspectRatio;
		OutsideColor = Color::Black;
		DisplayFlags = static_cast<hvn3::DisplayFlags>(0);

		FreezeWhenLostFocus = true;
		FrameRate = 60.0;
		MaxFrameSkip = 0;
		FixedFrameRate = true;

		DisplayCursor = true;
		ExitWithEscapeKey = true;

		DebugMode = false;

	}

}