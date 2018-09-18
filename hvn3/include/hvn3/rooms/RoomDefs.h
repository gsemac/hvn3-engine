#pragma once
#include "hvn3/core/SmartPointerToInterfaceWrapper.h"
#include <memory>

namespace hvn3 {

	class IRoom;
	class Room;

	typedef std::shared_ptr<IRoom> IRoomPtr;
	template <typename RoomType>
	using RoomPtr = hvn3::system::SmartPointerToInterfaceWrapper<IRoomPtr, RoomType>;
	typedef int RoomId;

	enum : RoomId {
		NULL_ROOM_ID = -1
	};

	template<typename RoomType = Room, typename... Args>
	RoomPtr<RoomType> make_room(Args&&... args) {

		IRoomPtr smart_ptr = std::make_shared<RoomType>(std::forward<Args>(args)...);
		RoomPtr<RoomType> handle(std::move(smart_ptr));

		return handle;

	}

}