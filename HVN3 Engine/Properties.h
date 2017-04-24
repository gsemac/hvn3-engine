#pragma once
#include "Size.h"
#include "Color.h"
#include "Display.h"
#include <string>

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
	bool Fullscreen;
	::ScalingMode ScalingMode;
	Color OutsideColor;
	::DisplayFlags DisplayFlags;

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