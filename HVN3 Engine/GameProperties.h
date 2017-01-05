#pragma once
#include "Size.h"
#include "Color.h"
#include <string>

enum class ScalingMode {
	Fixed,
	Full,
	MaintainAspectRatio
};

struct GameProperties {

public:
	GameProperties();

	// Display Properties

	std::string DisplayTitle;
	Size DisplaySize;
	bool Fullscreen;
	::ScalingMode ScalingMode;
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