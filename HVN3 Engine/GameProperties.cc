#include "GameProperties.h"

GameProperties::GameProperties() : 
	DisplaySize(640.0f, 480.0f) {

	DisplayTitle = "HVN3 Engine";
	Fullscreen = false;
	ScalingMode = ::ScalingMode::MaintainAspectRatio;
	OutsideColor = Color::Silver;

	FreezeWhenLostFocus = true;
	FPS = 60.0f;
	MaxFrameSkip = 0;

	DisplayCursor = true;
	ExitWithEscapeKey = true;

	DebugMode = false;

}