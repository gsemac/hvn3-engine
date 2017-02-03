#include "UpdateEventArgs.h"

UpdateEventArgs::UpdateEventArgs(float dt) : __delta_time(dt) {}

float UpdateEventArgs::Delta() const {

	return __delta_time;

}