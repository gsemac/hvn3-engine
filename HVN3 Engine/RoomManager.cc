#include "RoomManager.h"
#include "RoomController.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"
#include <utility>

namespace hvn3 {

	RoomManager::RoomManager() {

		_current_room = 0;

	}
	RoomManager::~RoomManager() {

		// Call the room exit event for the current room.
		if (_rooms.size() >= 1) {
			RoomController controller(CurrentRoom());
			controller.Reset();
			controller.CallRoomExitEvent(RoomExitEventArgs());
		}

		// Clear all rooms.
		_rooms.clear();

	}

	void RoomManager::RoomAdd(std::unique_ptr<Room>& room) {

		// Add the new room.
		_rooms.push_back(std::move(room));

		// If this is the first room we've added and it hasn't been set up, set it up.
		if (_rooms.size() == 1) {
			RoomController controller(CurrentRoom());
			controller.CallRoomEnterEvent(RoomEnterEventArgs());
			if (!controller.IsSetUp())
				controller.SetUp();
		}

	}
	void RoomManager::RoomLoad(RoomId id) {

		size_t index = FindRoomIndex(id);

		RoomLoad(index);

	}
	void RoomManager::LoadNext() {

		// If we're at the last room, loop back to the beginning.
		_next_room = _current_room + 1;
		if (_next_room >= _rooms.size())
			_next_room = 0;

		// Initiate a pending room change.
		_room_change_pending = true;

	}
	void RoomManager::LoadPrevious() {

		// If we're at the first room, loop around to the end.
		_next_room = _current_room == 0 ? _rooms.size() - 1 : --_current_room;

		// Initiate a pending room change.
		_room_change_pending = true;

	}
	void RoomManager::RestartRoom() {

		// Initiate a pending restart.
		_restart_pending = true;

	}
	Room& RoomManager::CurrentRoom() {

		return *_rooms[_current_room];

	}
	size_t RoomManager::CurrentRoomIndex() const {

		return _current_room;

	}
	size_t RoomManager::RoomCount() const {

		return _rooms.size();

	}

	void RoomManager::Update(UpdateEventArgs& e) {

		// Update the state of the room.
		CurrentRoom().Update(e);

		// If a room change is pending, change rooms now.
		if (_room_change_pending) {

			// Load the new room.
			RoomLoad(_next_room);

			// Disable the room change pending flag, along with any pending restarts.
			_room_change_pending = false;
			_restart_pending = false;

		}

		// If a restart is pending, perform the restart now.
		else if (_restart_pending) {

			// Create a controller object for the current room.
			RoomController controller(CurrentRoom());

			// Reset the state of the room.
			controller.Reset();

			// Set-up the room again.
			controller.SetUp();

			// Disable the restarting pending flag.
			_restart_pending = false;

		}

	}
	void RoomManager::Draw(DrawEventArgs& e) {

		// Draw the state of the room.
		CurrentRoom().Draw(e);

	}

	void RoomManager::RoomLoad(size_t room_index) {

		// Create room controllers for the current room and the next room.
		RoomController current_room_controller(CurrentRoom());
		RoomController next_room_controller(*_rooms[room_index]);

		// If the current room has been set up and isn't persistent, reset it.
		if (current_room_controller.IsSetUp() && !CurrentRoom().Persistent())
			current_room_controller.Reset();

		// Call the on exit event for the current room.
		current_room_controller.CallRoomExitEvent(RoomExitEventArgs());

		// Call the on enter event for the new room.
		next_room_controller.CallRoomEnterEvent(RoomEnterEventArgs());

		// If the next room hasn't been set up yet, set it up.
		if (!next_room_controller.IsSetUp())
			next_room_controller.SetUp();

		// Update current room index.
		_current_room = room_index;

	}
	size_t RoomManager::FindRoomIndex(RoomId id) const {

		// Look through the list for a room with the room id.
		for (size_t i = 0; i < _rooms.size(); ++i)
			if (_rooms[i]->Id() == id)
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

}