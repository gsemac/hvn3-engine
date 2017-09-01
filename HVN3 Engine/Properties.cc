#include "Properties.h"

namespace hvn3 {
	namespace System {

		Properties::Properties() :
			DisplaySize(640, 480) {

			DisplayTitle = "HVN3 Engine";
			StartFullscreen = false;
			ScalingMode = hvn3::ScalingMode::MaintainAspectRatio;
			OutsideColor = Color::Black;
			DisplayFlags = static_cast<hvn3::DisplayFlags>(0);

			FreezeWhenLostFocus = true;
			Fps = 60.0f;
			MaxFrameSkip = 0;
			FixedFrameRate = true;

			DisplayCursor = true;
			ExitWithEscapeKey = true;

			DebugMode = false;

		}

	}
}