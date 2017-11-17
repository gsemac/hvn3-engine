#pragma once
#include <vector>
#include <memory>
#include "rooms/Room.h"
#include "core/UpdateEventArgs.h"
#include "core/DrawEventArgs.h"
#include "IRoomManager.h"
#include "rooms/RoomTransitionNone.h"
#include "rooms/RoomTransitionFade.h"
#include "rooms/RoomController.h"

namespace hvn3 {

	class IRoomTransition;

	typedef Room room_type;
	class RoomManager : public IRoomManager {

		enum ROOM_TRANSITION_STATE : char {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

	public:
		typedef room_type room_type;

		RoomManager() : 
			_transition(nullptr) {

			_current_room = 0;
			_next_room = 0;
			_room_transition_state = NO_TRANSITION_PENDING; 

		}
		~RoomManager() {

			// Call the room exit event for the current room.
			ExitCurrentRoom();

			// Clear all rooms.
			_rooms.clear();

			// Release the transition object so there's no problem with it being freed twice.
			_transition.release();

		}

		void AddRoom(RoomPtr& room) {

			// Add the new room.
			_rooms.push_back(std::move(room));

			// If this is the first room we've added and it hasn't been set up, set it up.
			if (_rooms.size() == 1) {
				System::RoomController controller(CurrentRoom());
				controller.CallRoomEnterEvent(RoomEnterEventArgs());
				if (!controller.IsSetUp())
					controller.SetUp();
			}

		}
		void AddRoom(IRoom* room) {

			AddRoom(RoomPtr(room));

		}
		void GotoRoom(RoomId id) {

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
		void LoadNext() {

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
		void LoadPrevious() {

			// If a room transition is already in progress, do nothing.
			if (RoomTransitionInProgress())
				return;

			// If we're at the first room, loop around to the end.
			size_t room_index = _current_room == 0 ? _rooms.size() - 1 : --_current_room;

			// Initiate a pending room change.
			InitiatePendingRoomTransition(room_index);

		}

		void RestartRoom() {

			// Initiate a pending restart.
			if (CurrentRoom() != nullptr)
				CurrentRoom()->Restart();

		}
		void ClearRoom() {

			// Reset the state of the current room without calling any events.
			// #todo This should be pending; it shouldn't happen immediately.
			System::RoomController(CurrentRoom()).Reset();

		}
		room_type* CurrentRoom() {

			if (_current_room >= 0 && _current_room < _rooms.size())
				return static_cast<room_type*>(_rooms[_current_room].get());

			return nullptr;

		}
		size_t CurrentRoomIndex() const {

			return _current_room;

		}
		size_t RoomCount() const {

			return _rooms.size();

		}

		void SetRoomTransition(RoomTransitionPtr& transition) {

			_transition = std::move(transition);

		}
		void SetRoomTransition(RoomTransition transition) {

			switch (transition) {

			case RoomTransition::None:
				SetRoomTransition(RoomTransitionPtr(new RoomTransitionNone));
				break;

			case RoomTransition::Fade:
				SetRoomTransition(RoomTransitionPtr(new RoomTransitionFade(Color::Black, true)));
				break;

			}

		}

		void Update(UpdateEventArgs& e) {

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

		}
		void Draw(DrawEventArgs& e) {

			// Draw the state of the room.
			CurrentRoom()->OnDraw(e);

			// Draw the state of the room transition.
			if (RoomTransitionInProgress() && _transition)
				_transition->OnDraw(e);

		}

	protected:
		void LoadRoom(size_t room_index) {
			
			// Create room controllers for the current room and the next room.
			System::RoomController current_room_controller(CurrentRoom());
			System::RoomController next_room_controller(_rooms[room_index].get());

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
		size_t FindRoomIndex(RoomId id) const {

			// Look through the list for a room with the room id.
			for (size_t i = 0; i < _rooms.size(); ++i)
				if (_rooms[i]->Id() == id)
					return i;

			// Return the size of the rooms list if we can't find a room with the requested id.
			return _rooms.size();

		}
		size_t FindRoomIndex(const std::unique_ptr<IRoom>& room) const {

			// Look through the list for a room with the same pointer.
			for (size_t i = 0; i < _rooms.size(); ++i)
				if (_rooms[i] == room)
					return i;

			// Return the size of the rooms list if we can't find the requested room.
			return _rooms.size();

		}

		// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
		void ExitCurrentRoom() {

			// Call the room exit event for the current room.
			if (_rooms.size() >= 1) {
				System::RoomController controller(CurrentRoom());
				controller.Reset();
				controller.CallRoomExitEvent(RoomExitEventArgs());
			}

		}

		bool RoomTransitionInProgress() const {
			
			return _room_transition_state != NO_TRANSITION_PENDING;

		}
		void InitiatePendingRoomTransition(size_t room_index) {

			_next_room = room_index;
			_room_transition_state = TRANSITION_PENDING;

		}

	private:
		size_t _current_room;
		size_t _next_room;
		std::vector<std::unique_ptr<IRoom>> _rooms;
		ROOM_TRANSITION_STATE _room_transition_state;
		std::unique_ptr<IRoomTransition> _transition;

	};

}