#pragma once
#include <vector>
#include <memory>
#include "Room.h"

class RoomManager {

public:
	RoomManager();

	void RoomAdd(std::unique_ptr<Room>& room);
	void RoomLoad(std::unique_ptr<Room>& room);
	void RoomLoad(RoomId id);
	void LoadNext();
	void LoadPrevious();

	void RestartRoom();
	Room& CurrentRoom();

	size_t RoomCount() const;

private:
	size_t _current_room;
	std::vector<std::unique_ptr<Room>> _rooms;

	size_t FindRoomIndex(RoomId id) const;
	size_t FindRoomIndex(const std::unique_ptr<Room>& room) const;

};