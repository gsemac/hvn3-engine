#pragma once
#include "UpdateEventArgs.h"
#include "Size.h"

class ViewUpdateEventArgs : public UpdateEventArgs {

public:
	ViewUpdateEventArgs(float dt, Size room_size);
	const Size RoomSize() const;

private:
	Size _room_size;

};