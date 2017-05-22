#pragma once
#include <vector>
#include <memory>
#include "Room.h"
#include "UpdateEventArgs.h"
#include "DrawEventArgs.h"
#include "IRoomManager.h"

namespace hvn3 {

	class RoomManager : public IRoomManager {

	public:
		RoomManager();
		~RoomManager();

		void RoomAdd(std::unique_ptr<Room>& room);
		void RoomLoad(RoomId id);
		void LoadNext();
		void LoadPrevious();

		void RestartRoom();
		Room& CurrentRoom();
		size_t CurrentRoomIndex() const;
		size_t RoomCount() const;

		void Update(UpdateEventArgs& e);
		void Draw(DrawEventArgs& e);

	protected:
		void RoomLoad(size_t room_index);
		size_t FindRoomIndex(RoomId id) const;
		size_t FindRoomIndex(const std::unique_ptr<Room>& room) const;

	private:
		size_t _current_room;
		size_t _next_room;
		std::vector<std::unique_ptr<Room>> _rooms;
		bool _restart_pending;
		bool _room_change_pending;

	};

}