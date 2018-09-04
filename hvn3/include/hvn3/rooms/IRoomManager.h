#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/rooms/RoomTypeDefs.h"
#include <memory>

namespace hvn3 {

	class IRoomManager :
		public IDrawable,
		public IUpdatable {

	public:
		// Sets the current room to the given room object, and begins running it immediately.
		virtual void SetRoom(RoomPtr& room) = 0;
		// Returns the current room.
		virtual RoomPtr& Room() = 0;
		// Returns the current room.
		virtual const RoomPtr& Room() const = 0;
		// Deinitializes and reinitializes the current room.
		virtual void RestartRoom() = 0;
		// Clears all objects, backgrounds, etc. from the current room.
		virtual void ClearRoom() = 0;
		virtual bool IsRoomNull() = 0;

	};

}