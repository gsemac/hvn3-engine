#include "core/time_span.h"

namespace hvn3 {

	// Public members

	double TimeSpan::Seconds() const {

		return totalSeconds;

	}

	TimeSpan TimeSpan::FromSeconds(double totalSeconds) {

		return TimeSpan(totalSeconds);

	}

	// Private members

	TimeSpan::TimeSpan(double totalSeconds) {

		this->totalSeconds = totalSeconds;

	}

}