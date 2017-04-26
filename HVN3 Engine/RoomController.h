#pragma once
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"

class Room;

class RoomController {

public:
	RoomController(Room& room);

	void SetUp();
	bool IsSetUp() const;
	void SetSetUp(bool value);
	void Reset();

	void CallRoomEnterEvent(RoomEnterEventArgs& e);
	void CallRoomExitEvent(RoomExitEventArgs& e);

private:
	Room* _room;

};