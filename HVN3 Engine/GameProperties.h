#pragma once
#include "Size.h"
#include "Color.h"
#include <string>

struct GameProperties {

public:
	GameProperties();

	// Display Properties

	std::string DisplayTitle;
	Size DisplaySize;
	bool Fullscreen;
	bool MaintainAspectRatio;
	Color OutsideColor;

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