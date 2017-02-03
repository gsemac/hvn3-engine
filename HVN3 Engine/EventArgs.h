#pragma once
#include "EventSource.h"

// This class will eventually replace Event as more specific event types are derived from it.

class EventArgs {

public:
	EventArgs(const EventSource& source);

	const EventSource& Source() const;
	double Timestamp() const;

private:
	//EventSource __source;
	double __timestamp;

};