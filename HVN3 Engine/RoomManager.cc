#include "RoomManager.h"
#include <utility>

RoomManager::RoomManager() {

	_current_room = 0;

}
void RoomManager::RoomAdd(std::unique_ptr<Room>& room) {

	_rooms.push_back(std::move(room));

}
void RoomManager::RoomLoad(std::unique_ptr<Room>& room) {

	// Attempt to find the room in the list.
	_current_room = FindRoomIndex(room);

	// If we couldn't find the room, add it.
	if (_current_room == _rooms.size())
		RoomAdd(room);

	// Reset the state of the room.
	RestartRoom();

}
void RoomManager::RoomLoad(RoomId id) {

	_current_room = FindRoomIndex(id);

	RoomLoad(_rooms[_current_room]);

}
void RoomManager::LoadNext() {

	if (_current_room + 1 < _rooms.size())
		RoomLoad(_rooms[++_current_room]);

}
void RoomManager::LoadPrevious() {

	if (_current_room > 0)
		RoomLoad(_rooms[--_current_room]);

}
void RoomManager::RestartRoom() {

	_rooms[_current_room]->Restart();

}
Room& RoomManager::CurrentRoom() {

	return *_rooms[_current_room];

}
size_t RoomManager::RoomCount() const {

	return _rooms.size();

}

size_t RoomManager::FindRoomIndex(RoomId id) const {

	// Look through the list for a room with the room id.
	for (size_t i = 0; i < _rooms.size(); ++i)
		if (_rooms[i]->RoomId() == id)
			return i;

	// Return the size of the rooms list if we can't find a room with the requested id.
	return _rooms.size();

}
size_t RoomManager::FindRoomIndex(const std::unique_ptr<Room>& room) const {

	// Look through the list for a room with the same pointer.
	for (size_t i = 0; i < _rooms.size(); ++i)
		if (_rooms[i] == room)
			return i;

	// Return the size of the rooms list if we can't find the requested room.
	return _rooms.size();

}