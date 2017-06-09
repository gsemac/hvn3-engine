#include "RoomManager.h"
#include "RoomController.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"
#include "RoomTransitionNone.h"
#include <utility>

namespace hvn3 {

	RoomManager::RoomManager() :
		_transition(nullptr) {

		_current_room = 0;
		_next_room = 0;
		_room_transition_state = NO_TRANSITION_PENDING;
		_restart_pending = false;

	}
	RoomManager::~RoomManager() {

		// Call the room exit event for the current room.
		ExitCurrentRoom();

		// Clear all rooms.
		_rooms.clear();

		// Release the transition object so there's no problem with it being freed twice.
		_transition.release();

	}

	void RoomManager::AddRoom(std::unique_ptr<Room>& room) {

		// Add the new room.
		_rooms.push_back(std::move(room));

		// If this is the first room we've added and it hasn't been set up, set it up.
		if (_rooms.size() == 1) {
			RoomController controller(*CurrentRoom());
			controller.CallRoomEnterEvent(RoomEnterEventArgs());
			if (!controller.IsSetUp())
				controller.SetUp();
		}

	}

	void RoomManager::GotoRoom(RoomId id) {

		// If a room transition is already in progress, do nothing.
		if (RoomTransitionInProgress())
			return;

		// Find the index of the requested room.
		size_t room_index = FindRoomIndex(id);

		// If the room doesn't exist, do nothing.
		if (room_index >= _rooms.size())
			return;

		// Otherwise, initiate a pending room change.
		InitiatePendingRoomTransition(room_index);

	}
	void RoomManager::LoadNext() {

		// If a room transition is already in progress, do nothing.
		if (RoomTransitionInProgress())
			return;

		// If we're at the last room, loop back to the beginning.
		size_t room_index = _current_room + 1;
		if (room_index >= _rooms.size())
			room_index = 0;

		// Initiate a pending room change.
		InitiatePendingRoomTransition(room_index);

	}
	void RoomManager::LoadPrevious() {

		// If a room transition is already in progress, do nothing.
		if (RoomTransitionInProgress())
			return;

		// If we're at the first room, loop around to the end.
		size_t room_index = _current_room == 0 ? _rooms.size() - 1 : --_current_room;

		// Initiate a pending room change.
		InitiatePendingRoomTransition(room_index);

	}

	void RoomManager::RestartRoom() {

		// Initiate a pending restart.
		_restart_pending = true;

	}
	void RoomManager::ClearRoom() {

		// Reset the state of the current room without calling any events.
		// #todo This should be pending; it shouldn't happen immediately.
		RoomController(*CurrentRoom()).Reset();

	}

	Room* RoomManager::CurrentRoom() {

		if (_current_room >= 0 && _current_room < _rooms.size())
			return _rooms[_current_room].get();

		return nullptr;

	}
	size_t RoomManager::CurrentRoomIndex() const {

		return _current_room;

	}
	size_t RoomManager::RoomCount() const {

		return _rooms.size();

	}

	void RoomManager::Update(UpdateEventArgs& e) {

		// Update the state of the room if permitted.
		if (!RoomTransitionInProgress() || !_transition || _transition->AllowRoomUpdate())
			CurrentRoom()->OnUpdate(e);

		switch (_room_transition_state) {

		case TRANSITION_PENDING:

			// If the transition is null (no transition), just perform the change immediately.
			if (!_transition) {
				LoadRoom(_next_room);
				_room_transition_state = NO_TRANSITION_PENDING;
				break;
			}

			// Begin transitioning to the next room.
			_transition->ExitBegin();
			_room_transition_state = EXIT_IN_PROGRESS;
			break;

		case EXIT_IN_PROGRESS:

			// Update the state of the room transition. 
			// If the transition is complete, load the next room and begin the enter transition.
			if (_transition->ExitStep(e)) {
				_transition->ExitEnd();
				LoadRoom(_next_room);
				_transition->EnterBegin();
				_room_transition_state = ENTER_IN_PROGRESS;
			}
			break;

		case ENTER_IN_PROGRESS:
			// Update the state of the room transition. 
			// If the transition is complete, reset the transition state to allow future room changes.
			if (_transition->EnterStep(e)) {
				_transition->EnterEnd();
				_room_transition_state = NO_TRANSITION_PENDING;
			}
			break;

		}

		if (_restart_pending) {

			// Create a controller object for the current room.
			RoomController controller(*CurrentRoom());

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
		CurrentRoom()->OnDraw(e);

		// Draw the state of the room transition.
		if (RoomTransitionInProgress() && _transition)
			_transition->OnDraw(e);

	}

	// Protected methods

	void RoomManager::LoadRoom(size_t room_index) {

		// Create room controllers for the current room and the next room.
		RoomController current_room_controller(*CurrentRoom());
		RoomController next_room_controller(*_rooms[room_index]);

		// If the current room has been set up and isn't persistent, reset it.
		if (current_room_controller.IsSetUp() && !CurrentRoom()->Persistent())
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

	void RoomManager::ExitCurrentRoom() {

		// Call the room exit event for the current room.
		if (_rooms.size() >= 1) {
			RoomController controller(*CurrentRoom());
			controller.Reset();
			controller.CallRoomExitEvent(RoomExitEventArgs());
		}

	}
	bool RoomManager::RoomTransitionInProgress() const {

		return _room_transition_state != NO_TRANSITION_PENDING;

	}
	void RoomManager::InitiatePendingRoomTransition(size_t room_index) {

		_next_room = room_index;
		_room_transition_state = TRANSITION_PENDING;

	}

}