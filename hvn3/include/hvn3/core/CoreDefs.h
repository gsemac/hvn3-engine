#pragma once
#include <cassert>

#define HVN3_BREAK return hvn3::BREAK
#define HVN3_CONTINUE return hvn3::CONTINUE

#define HVN3_ASSERT(expression, message)\
	assert(((void)(message), (expression)));

namespace hvn3 {

	enum class ScalingMode {
		//Display contents are not scaled.
		Fixed,
		// Display contents are stretched to fit the exact size of the display area.
		Full,
		// Display contents are scaled to maintain the original aspect ratio.
		MaintainAspectRatio
	};

	const bool BREAK = false;
	const bool CONTINUE = true;

}