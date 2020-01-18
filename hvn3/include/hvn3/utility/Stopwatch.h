#pragma once
#include <chrono>


namespace hvn3 {

	class Stopwatch {

	public:
		Stopwatch();
		Stopwatch(bool started);

		// Starts the stopwatch without resetting the time elapsed.
		void Start();
		// Stops the stopwatch.
		void Stop();
		// Clears the time elapsed without stopping the stopwatch.
		void Clear();
		// Returns true if the stopwatch has been started.
		bool IsStarted() const;

		int64_t NanoSecondsElapsed();
		double MicroSecondsElapsed();
		double MilliSecondsElapsed();
		double SecondsElapsed();
		double MinutesElapsed();

	private:
		bool _started;
		int64_t _nanosecondsElapsed;
		std::chrono::time_point<std::chrono::steady_clock> _start;
		std::chrono::time_point<std::chrono::steady_clock> _end;

	};

}