#include "hvn3/exceptions/Exception.h"
#include "hvn3/rooms/RoomManager.h"
#include "hvn3/rooms/RoomProxy.h"
#include "hvn3/rooms/RoomTransitionNone.h"
#include "hvn3/rooms/RoomTransitionFade.h"

namespace hvn3 {

	RoomManager::RoomManager(System::IContextProvider* context_provider) :
		_transition(nullptr),
		_context_provider(context_provider) {

		_room_transition_state = NO_TRANSITION_PENDING;

	}
	RoomManager::~RoomManager() {

		// Resets the current room and calls its exit event (so the event is still called properly when the game ends).
		_exitRoom();

		/*
		Some implementations of GameManager may call the RoomManager destructor twice.
		To avoid freeing the transition object more than once, release it here.
		(?) I don't actually remember why I had to do this. Is it still necessary?
		*/
		_transition.release();

	}
	void RoomManager::SetRoom(IRoomPtr& room) {

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
	IRoomPtr& RoomManager::Room() {
		return _current_room;
	}
	const IRoomPtr& RoomManager::Room() const {
		return _current_room;
	}
	void RoomManager::RestartRoom() {

		// Initiate a pending restart.
		if (!IsRoomNull())
			System::RoomProxy(*Room()).Restart();

	}
	void RoomManager::ClearRoom() {

		// Reset the state of the current room without calling any events.
		// #todo This should be pending; it shouldn't happen immediately.
		if (!IsRoomNull())
			System::RoomProxy(*Room()).Reset();

	}
	bool RoomManager::IsRoomNull() {
		return !static_cast<bool>(_current_room);
	}
	void RoomManager::SetRoomTransition(RoomTransitionPtr& transition) {
		_transition = std::move(transition);
	}
	void RoomManager::SetRoomTransition(RoomTransition transition) {

		switch (transition) {

		case RoomTransition::None:
			SetRoomTransition(RoomTransitionPtr(new RoomTransitionNone));
			break;

		case RoomTransition::Fade:
			SetRoomTransition(RoomTransitionPtr(new RoomTransitionFade(Color::Black, true)));
			break;

		}

	}
	void RoomManager::OnUpdate(UpdateEventArgs& e) {

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
	void RoomManager::OnDraw(DrawEventArgs& e) {

		// Draw the state of the room.
		if (!IsRoomNull())
			Room()->OnDraw(e);

		// Draw the state of the room transition.
		if (_roomTransitionIsInProgress() && _transition)
			_transition->OnDraw(e);

	}

	void RoomManager::_loadNextRoom() {

		if (!IsRoomNull()) {

			System::RoomProxy proxy(*Room());

			// If the current room has been set up and isn't persistent, reset it.
			if (proxy.IsReady() && !_current_room->Persistent())
				proxy.Reset();

			// Call the on exit event for the current room.
			proxy.DispatchEvent(RoomExitEventArgs());

		}

		// Update current room.
		_current_room = std::move(_next_room);

		System::RoomProxy proxy(*Room());
		
		_context_provider->ProvideContext(*Room());

		// Call the on enter event for the new room.
		proxy.DispatchEvent(RoomEnterEventArgs());

		// If the next room hasn't been set up yet, set it up.
		if (!proxy.IsReady())
			proxy.SetUp();

	}
	// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
	void RoomManager::_exitRoom() {

		if (!IsRoomNull()) {

			System::RoomProxy proxy(*Room());

			proxy.Reset();
			proxy.DispatchEvent(RoomExitEventArgs());

			_current_room.reset();

		}

	}
	bool RoomManager::_roomTransitionIsInProgress() const {

		return _room_transition_state != NO_TRANSITION_PENDING;

	}
	void RoomManager::_beginRoomTransition(IRoomPtr& next_room) {

		_next_room = next_room;
		_room_transition_state = TRANSITION_PENDING;

	}

}