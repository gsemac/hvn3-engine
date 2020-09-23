#include "hvn3/utility/FpsCounter.h"

namespace hvn3 {

	FpsCounter::FpsCounter() :
		FpsCounter(0.0) { // Passing in 0 will produce instaneous updates
	}
	FpsCounter::FpsCounter(double updatesPerSecond) {

		_frame_count = 0;

		_updates_per_second = updatesPerSecond;
		_seconds_elapsed = 0.0;

		_fps_sum = 0.0;
		_average_fps = 0.0;
		_seconds_per_frame = 0.0;

	}

	double FpsCounter::AverageFps() const {
		return _average_fps;
	}
	double FpsCounter::SecondsPerFrame() const {
		return _seconds_per_frame;
	}
	double FpsCounter::NextFrame() {

		// If the timer hasn't started yet, start it.

		if (!_frame_timer.IsStarted())
			_frame_timer.Start();

		++_frame_count;

		// Calculate the instaneous framerate according to how long the last frame took, and add it to our sum.

		double instantaneous_fps = 1.0 / _frame_timer.SecondsElapsed();
		_fps_sum += instantaneous_fps;

		_seconds_elapsed += _frame_timer.SecondsElapsed();

		if (_seconds_elapsed >= _updates_per_second) {

			// If the update period has been reached, update all averages.

			_average_fps = _fps_sum / static_cast<double>(_frame_count);
			_seconds_per_frame = _seconds_elapsed / static_cast<double>(_frame_count);

			// Reset sums for the next set of frames.

			_frame_count = 0;
			_fps_sum = 0.0;
			_seconds_elapsed = 0.0;

		}

		// Reset the frame timer for the next frame.
		_frame_timer.Clear();

		return instantaneous_fps;

	}

}