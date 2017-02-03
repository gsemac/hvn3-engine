#pragma once
#include "EventArgs.h"

class UpdateEventArgs : public EventArgs {

public:
	UpdateEventArgs(float dt);

	// Returns the number of seconds elapsed since the last update.
	float Delta() const;

private:
	float __delta_time;

};