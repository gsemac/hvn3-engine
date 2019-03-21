#pragma once
#include "hvn3/utility/Stopwatch.h"

namespace hvn3 {

	class FpsCounter {

	public:
		FpsCounter();
		FpsCounter(double updatesPerSecond);

		// Returns the average frames per second rate.
		double AverageFps() const;
		// Returns the average number of seconds spent per frame.
		double SecondsPerFrame() const;

		// Resets the timer to begin timing the next frame, returning the time spent (in seconds) on the previous frame.
		double NextFrame();

	private:
		Stopwatch _frame_timer;
		unsigned long _frame_count;

		double _updates_per_second;
		double _seconds_elapsed;

		double _fps_sum;
		double _average_fps;
		double _seconds_per_frame;

	};

}