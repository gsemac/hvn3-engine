#pragma once
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"

class RoomBase;

class RoomController {

public:
	RoomController(RoomBase& room);

	void SetUp();
	bool IsSetUp() const;
	void SetSetUp(bool value);
	void Reset();

	void CallRoomEnterEvent(RoomEnterEventArgs& e);
	void CallRoomExitEvent(RoomExitEventArgs& e);

private:
	RoomBase* _room;

};