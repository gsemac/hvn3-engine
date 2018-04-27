#pragma once
#include <memory>

namespace hvn3 {

	class IRoom;

	typedef int RoomId;
	typedef std::unique_ptr<IRoom> RoomPtr;

	enum : RoomId {
		NULL_ROOM_ID = -1
	};

}