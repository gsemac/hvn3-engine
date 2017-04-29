#pragma once
#include "Room.h"
#include "BackgroundManager.h"

// This class extends the responsibilites of the Room class to include background and tile management.
class RoomExt : public Room {

public:
	IBackgroundManager& BackgroundManager();

private:
	::BackgroundManager _background_manager;

};