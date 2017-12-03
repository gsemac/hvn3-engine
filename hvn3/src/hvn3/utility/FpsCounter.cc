#include "hvn3/utility/FpsCounter.h"

namespace hvn3 {
	namespace System {

		FpsCounter::FpsCounter() :
			FpsCounter(1.0) {}
		FpsCounter::FpsCounter(double update_rate) {

			_frame_count = 0;

			_update_rate = update_rate;
			_time_elapsed = 0.0;

			_fps_sum = 0.0;
			_average_fps = 0.0;
			_aggregate_fps = 0.0;

		}

		double FpsCounter::AverageFps() const {

			return _average_fps;

		}
		double FpsCounter::AggregateFps() const {

			return _aggregate_fps;

		}
		double FpsCounter::NextFrame() {

			if (!_frame_timer.IsTiming())
				_frame_timer.Start();

			++_frame_count;

			double instantaneous_fps = 1.0 / _frame_timer.SecondsElapsed();
			_fps_sum += instantaneous_fps;

			_time_elapsed += _frame_timer.SecondsElapsed();

			if (_time_elapsed >= _update_rate) {

				_aggregate_fps = (double)_frame_count / _frame_timer.SecondsElapsed();
				_average_fps = _fps_sum / (double)_frame_count;

				_frame_count = 0;
				_fps_sum = 0.0;
				_time_elapsed = 0.0;

			}

			_frame_timer.Reset();

			return instantaneous_fps;

		}

	}
}