#include "hvn3/utility/Stopwatch.h"

namespace hvn3 {

	typedef std::chrono::high_resolution_clock clock_type;

	Stopwatch::Stopwatch() :
		Stopwatch(false) {
	}
	Stopwatch::Stopwatch(bool start) {

		// Initialize the starting duration to 0 ns.
		_nanosecondsElapsed = 0;

		// The stopwatch is not currently running.
		_started = false;

		// Start the stopwatch immediately if requested.

		if (start)
			Start();

	}
	void Stopwatch::Start() {

		if (!_started) {

			// The stopwatch is now running.
			_started = true;

			// Set the starting time.
			_start = clock_type::now();

		}

	}
	void Stopwatch::Stop() {

		// If the stopwatch is not running, do nothing.
		if (!_started)
			return;

		// The stopwatch is no longer running.
		_started = false;

		// Update the time elapsed.
		_nanosecondsElapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - _start).count();

	}
	void Stopwatch::Clear() {

		// Reset the time elapsed.
		_nanosecondsElapsed = 0;

		// Reset the starting time, in case the stopwatch is running.
		_start = clock_type::now();

	}
	bool Stopwatch::IsStarted() const {

		return _started;

	}

	int64_t Stopwatch::NanoSecondsElapsed() {

		if (_started)
			return _nanosecondsElapsed + std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - _start).count();
		else
			return _nanosecondsElapsed;

	}
	double Stopwatch::MicroSecondsElapsed() {

		return NanoSecondsElapsed() / 1.e+3;

	}
	double Stopwatch::MilliSecondsElapsed() {

		return NanoSecondsElapsed() / 1.e+6;

	}
	double Stopwatch::SecondsElapsed() {

		return NanoSecondsElapsed() / 1.e+9;

	}
	double Stopwatch::MinutesElapsed() {

		return NanoSecondsElapsed() / 6.e+10;

	}

}