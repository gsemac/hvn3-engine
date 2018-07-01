#pragma once
#include "hvn3/core/IContextProvider.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/rooms/IRoomManager.h"
#include "hvn3/rooms/RoomTransitionBase.h"
#include "hvn3/rooms/RoomTransitionNone.h"
#include "hvn3/rooms/RoomTransitionFade.h"
#include <vector>

namespace hvn3 {

	class RoomManager : public IRoomManager {

		enum ROOM_TRANSITION_STATE {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

	public:
		RoomManager(System::IContextProvider* context_provider) :
			_transition(nullptr),
			_context_provider(context_provider) {

			_room_transition_state = NO_TRANSITION_PENDING;

		}
		~RoomManager() {

			// Resets the current room and calls its exit event (so the event is still called properly when the game ends).
			_exitRoom();

			/*
			Some implementations of GameManager may call the RoomManager destructor twice.
			To avoid freeing the transition object more than once, release it here.
			(?) I don't actually remember why I had to do this. Is it still necessary?
			*/
			_transition.release();

		}

		void SetRoom(RoomPtr& room) override {

			if (_roomTransitionIsInProgress())
				return;

			// If no room has been loaded, load the first room immediately without a transition.
			if (IsRoomNull()) {
				_next_room = room;
				_loadNextRoom();
			}
			else
			_beginRoomTransition(room);

		}
		IRoom& GetRoom() override {

			if (_current_room)
				return *_current_room;

			throw System::NullReferenceException("Attempted to get the current room when no room has been loaded.");

		}
		void RestartRoom() override {

			// Initiate a pending restart.
			if (_current_room)
				_current_room->Restart();

		}
		void ClearRoom() override {

			// Reset the state of the current room without calling any events.
			// #todo This should be pending; it shouldn't happen immediately.
			if (_current_room)
				_current_room->OnReset();

		}
		bool IsRoomNull() override {
			return !static_cast<bool>(_current_room);
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

		void OnUpdate(UpdateEventArgs& e) override {
		
			// Update the state of the room if there is nothing blocking updates.
			if (_current_room && (!_roomTransitionIsInProgress() || !_transition || !_transition->Blocking()))
				_current_room->OnUpdate(e);

			switch (_room_transition_state) {

			case TRANSITION_PENDING:
			
				// If the transition is null (no transition), just perform the change immediately.
				if (!_transition) {
					_loadNextRoom();
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
					_loadNextRoom();
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
		void OnDraw(DrawEventArgs& e) override {

			// Draw the state of the room.
			if (!IsRoomNull())
				GetRoom().OnDraw(e);

			// Draw the state of the room transition.
			if (_roomTransitionIsInProgress() && _transition)
				_transition->OnDraw(e);

		}

	protected:
		void _loadNextRoom() {

			if (!IsRoomNull()) {

				// If the current room has been set up and isn't persistent, reset it.
				if (_current_room->IsReady() && !_current_room->Persistent())
					_current_room->OnReset();

				// Call the on exit event for the current room.
				_current_room->OnExit(RoomExitEventArgs());

			}

			_next_room->SetContext(_context_provider->Context());

			// Call the on enter event for the new room.
			_next_room->OnEnter(RoomEnterEventArgs());

			// If the next room hasn't been set up yet, set it up.
			if (!_next_room->IsReady())
				_next_room->OnCreate();
	
			// Update current room.
			_current_room = std::move(_next_room);

		}
		// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
		void _exitRoom() {

			if (!IsRoomNull()) {
				
				GetRoom().OnReset();
				GetRoom().OnExit(RoomExitEventArgs());

			}

		}

		bool _roomTransitionIsInProgress() const {

			return _room_transition_state != NO_TRANSITION_PENDING;

		}
		void _beginRoomTransition(RoomPtr& next_room) {

			_next_room = next_room;
			_room_transition_state = TRANSITION_PENDING;

		}

	private:
		RoomPtr _current_room;
		RoomPtr _next_room;
		System::IContextProvider* _context_provider;
		ROOM_TRANSITION_STATE _room_transition_state;
		std::unique_ptr<RoomTransitionBase> _transition;

	};

}