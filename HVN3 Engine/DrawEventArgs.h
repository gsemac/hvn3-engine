#pragma once
#include "Event.h"
#include "Graphics.h"

class DrawEventArgs : public Event {

public:
	DrawEventArgs(Drawing::Graphics& graphics);
	Drawing::Graphics& Graphics();

private:
	Drawing::Graphics& __graphics;
	
};
