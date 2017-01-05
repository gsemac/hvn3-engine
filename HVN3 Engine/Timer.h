#pragma once
#include <allegro5/allegro.h>
#include "EventSource.h"

class Timer {

public:
	Timer(double seconds_per_tick);
	~Timer();
	void Start();
	void Resume();
	void Stop();
	bool HasStarted() const;
	int64_t Count() const;
	void SetCount(int64_t count);
	void AddCount(int64_t diff);
	double SecondsPerTick() const;
	void SetSecondsPerTick(double seconds_per_tick);
	EventSource EventSource();

private:
	ALLEGRO_TIMER* __timer;

};