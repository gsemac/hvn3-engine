#ifndef __STOPWATCH_H
#define __STOPWATCH_H
#include <chrono>

class Stopwatch {

private:
	bool __running, __initialized;
	std::chrono::time_point<std::chrono::steady_clock> __start;
	std::chrono::time_point<std::chrono::steady_clock> __end;

public:
	Stopwatch(bool start = false);
	void Start();
	void Stop();
	void Reset(bool start = false);
	long long NanoSecondsElapsed();
	long double MicroSecondsElapsed();
	long double MilliSecondsElapsed();
	long double SecondsElapsed();
	long double MinutesElapsed();

};

#endif