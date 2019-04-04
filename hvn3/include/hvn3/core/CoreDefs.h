#pragma once
#include <cassert>

#define HVN3_BREAK return hvn3::BREAK
#define HVN3_CONTINUE return hvn3::CONTINUE

#define HVN3_ASSERT(expression, message)\
	assert(((void)(message), (expression)));

namespace hvn3 {

	enum class ScalingMode {
		// No scaling is performed.
		None,
		//Display contents are not scaled, but are centered if the contents are smaller than the display area.
		Fixed,
		// Display contents are stretched to fit the exact size of the display area.
		Full,
		// Display contents are scaled to fit the display area while maintaining the aspect ratio.
		MaintainAspectRatio
	};

	const bool BREAK = false;
	const bool CONTINUE = true;

}