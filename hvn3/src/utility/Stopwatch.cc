#include "utility/Stopwatch.h"

namespace hvn3 {

	typedef std::chrono::high_resolution_clock clock_type;

	Stopwatch::Stopwatch(bool start_immediately) {

		// Initialize the starting duration to 0 ns.
		_nanoseconds_elapsed = 0;

		// The stopwatch is not currently running.
		_running = false;

		// Start the stopwatch immediately if requested.
		if (start_immediately)
			Start();

	}
	void Stopwatch::Start() {

		// If the stopwatch is running, do nothing.
		if (_running)
			return;

		// The stopwatch is now running.
		_running = true;

		// Set the starting time.
		_start = clock_type::now();

	}
	void Stopwatch::Stop() {

		// If the stopwatch is not running, do nothing.
		if (!_running)
			return;

		// The stopwatch is no longer running.
		_running = false;

		// Update the time elapsed.
		_nanoseconds_elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - _start).count();

	}
	void Stopwatch::Reset() {

		// Reset the time elapsed.
		_nanoseconds_elapsed = 0;

		// Reset the starting time, in case the stopwatch is running.
		_start = clock_type::now();
		
	}
	bool Stopwatch::IsTiming() const {

		return _running;

	}

	int64_t Stopwatch::NanoSecondsElapsed() {

		if (_running)
			return _nanoseconds_elapsed + std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - _start).count();
		else
			return _nanoseconds_elapsed;

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