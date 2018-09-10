#pragma once

#define HVN3_BREAK return hvn3::BREAK
#define HVN3_CONTINUE return hvn3::CONTINUE

namespace hvn3 {

	enum class ScalingMode {
		Fixed,
		Full,
		MaintainAspectRatio
	};

	const bool BREAK = false;
	const bool CONTINUE = true;

}