#pragma once
#include <memory>

namespace hvn3 {

	class IRoom;
	class Room;

	typedef int RoomId;
	typedef std::shared_ptr<IRoom> RoomPtr;

	enum : RoomId {
		NULL_ROOM_ID = -1
	};

	template<typename room_type = Room, typename... Args>
	RoomPtr make_room(Args&&... args) {

		RoomPtr ptr = std::make_shared<room_type>(std::forward<Args>(args)...);

		return ptr;

	}

}