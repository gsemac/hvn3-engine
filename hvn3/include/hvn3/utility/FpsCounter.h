#pragma once
#include "hvn3/utility/Stopwatch.h"

namespace hvn3 {
	namespace System {

		class FpsCounter {

		public:
			FpsCounter();
			FpsCounter(double update_rate);

			// Returns the average FPS.
			double AverageFps() const;
			// Returns the aggregate FPS.
			double AggregateFps() const;
			// Advances the frame counter and returns the instantaneous FPS for the current frame.
			double NextFrame();

		private:
			Stopwatch _frame_timer;
			unsigned long _frame_count;
			
			double _update_rate;
			double _time_elapsed;

			double _fps_sum;
			double _average_fps;
			double _aggregate_fps;

		};

	}
}