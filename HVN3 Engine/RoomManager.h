#pragma once
#include <vector>
#include <memory>
#include "Room.h"

class RoomManager {

public:
	RoomManager();

	void AddRoom(std::shared_ptr<Room> room);
	void LoadRoom(std::shared_ptr<Room> room);
	void LoadRoom(RoomId id);
	void LoadNext();
	void LoadPrevious();

	void RestartRoom();
	Room& CurrentRoom();

	size_t RoomCount() const;

private:
	size_t _current_room;
	std::vector<std::shared_ptr<Room>> _rooms;

	size_t FindRoomIndex(RoomId id) const;
	size_t FindRoomIndex(const std::shared_ptr<Room>& room) const;

};