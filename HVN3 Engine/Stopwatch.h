#pragma once
#include <chrono>


namespace hvn3 {

	class Stopwatch {

	public:
		Stopwatch(bool start_immediately = false);
		
		void Start();
		void Stop();
		void Reset();
		bool IsTiming() const;

		int64_t NanoSecondsElapsed();
		double MicroSecondsElapsed();
		double MilliSecondsElapsed();
		double SecondsElapsed();
		double MinutesElapsed();

	private:
		bool _running;
		int64_t _nanoseconds_elapsed;
		std::chrono::time_point<std::chrono::steady_clock> _start;
		std::chrono::time_point<std::chrono::steady_clock> _end;

	};

}