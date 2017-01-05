#include "Stopwatch.h"
typedef std::chrono::high_resolution_clock Clock;

Stopwatch::Stopwatch(bool start) {

	__initialized = false;

	if (start)
		Start();

}
void Stopwatch::Start() {

	__running = true;

	if (!__initialized) {
		__start = Clock::now();
		__initialized = true;
	}

}
void Stopwatch::Stop() {

	__running = false;
	__end = Clock::now();

}
void Stopwatch::Reset(bool start) {

	__start = Clock::now();
	__end = Clock::now();
	__running = false;

	if (start)
		Start();
	
}

long long Stopwatch::NanoSecondsElapsed() {

	if (__running)
		return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - __start).count();
	else
		return std::chrono::duration_cast<std::chrono::nanoseconds>(__end - __start).count();

}
long double Stopwatch::MicroSecondsElapsed() {

	return NanoSecondsElapsed() / 1000.0F;

}
long double Stopwatch::MilliSecondsElapsed() {

	return MicroSecondsElapsed() / 1000.0F;

}
long double Stopwatch::SecondsElapsed() {

	return MilliSecondsElapsed() / 1000.0F;

}
long double Stopwatch::MinutesElapsed() {

	return SecondsElapsed() / 60.0F;

}