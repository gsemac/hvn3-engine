//#include "hvn3/exceptions/Exception.h"
//#include "hvn3/rooms/RoomManager.h"
//#include "hvn3/rooms/RoomTransition.h"
//
//namespace hvn3 {
//
//	RoomManager::RoomManager() :
//		_transition(nullptr) {
//
//		_room_transition_state = NO_TRANSITION_PENDING;
//
//	}
//	RoomManager::~RoomManager() {
//
//		// Resets the current room and calls its exit event (so the event is still called properly when the game ends).
//		_exitRoom();
//
//		/*
//		Some implementations of GameManager may call the RoomManager destructor twice.
//		To avoid freeing the transition object more than once, release it here.
//		(?) I don't actually remember why I had to do this. Is it still necessary?
//		*/
//		_transition.release();
//
//	}
//	void RoomManager::SetRoom(IRoomPtr& room) {
//
//		if (_roomTransitionIsInProgress())
//			return;
//
//		// If no room has been loaded, load the first room immediately without a transition.
//		if (IsRoomNull()) {
//			_next_room = room;
//			_loadNextRoom();
//		}
//		else
//			_beginRoomTransition(room);
//
//	}
//	IRoomPtr& RoomManager::Room() {
//		return _current_room;
//	}
//	const IRoomPtr& RoomManager::Room() const {
//		return _current_room;
//	}
//	void RoomManager::RestartRoom() {
//
//		// Initiate a pending restart.
//		if (!IsRoomNull())
//			Room()->Restart();
//
//	}
//	void RoomManager::ClearRoom() {
//
//		// Reset the state of the current room without calling any events.
//		// #todo This should be pending; it shouldn't happen immediately.
//		if (!IsRoomNull())
//			Room()->OnReset();
//
//	}
//	bool RoomManager::IsRoomNull() {
//		return !static_cast<bool>(_current_room);
//	}
//	void RoomManager::SetRoomTransition(RoomTransitionPtr& transition) {
//		_transition = std::move(transition);
//	}
//	void RoomManager::SetRoomTransition(RoomTransition transition) {
//
//		switch (transition) {
//
//		case RoomTransition::None:
//			SetRoomTransition(RoomTransitionPtr(new RoomTransitionNone));
//			break;
//
//		case RoomTransition::Fade:
//			SetRoomTransition(RoomTransitionPtr(new RoomTransitionFade(Color::Black, true)));
//			break;
//
//		}
//
//	}
//	void RoomManager::OnUpdate(UpdateEventArgs& e) {
//
//		// Update the state of the room if there is nothing blocking updates.
//		if (_current_room && (!_roomTransitionIsInProgress() || !_transition || !_transition->Blocking()))
//			_current_room->OnUpdate(e);
//
//		switch (_room_transition_state) {
//
//		case TRANSITION_PENDING:
//
//			// If the transition is null (no transition), just perform the change immediately.
//			if (!_transition) {
//				_loadNextRoom();
//				_room_transition_state = NO_TRANSITION_PENDING;
//				break;
//			}
//
//			// Begin transitioning to the next room.
//			_transition->ExitBegin();
//			_room_transition_state = EXIT_IN_PROGRESS;
//			break;
//
//		case EXIT_IN_PROGRESS:
//
//			// Update the state of the room transition. 
//			// If the transition is complete, load the next room and begin the enter transition.
//			if (_transition->ExitStep(e)) {
//				_transition->ExitEnd();
//				_loadNextRoom();
//				_transition->EnterBegin();
//				_room_transition_state = ENTER_IN_PROGRESS;
//			}
//			break;
//
//		case ENTER_IN_PROGRESS:
//			// Update the state of the room transition. 
//			// If the transition is complete, reset the transition state to allow future room changes.
//			if (_transition->EnterStep(e)) {
//				_transition->EnterEnd();
//				_room_transition_state = NO_TRANSITION_PENDING;
//			}
//			break;
//
//		}
//
//	}
//	void RoomManager::OnDraw(DrawEventArgs& e) {
//
//		// Draw the state of the room.
//		if (!IsRoomNull())
//			Room()->OnDraw(e);
//
//		// Draw the state of the room transition.
//		if (_roomTransitionIsInProgress() && _transition)
//			_transition->OnDraw(e);
//
//	}
//	void RoomManager::OnContextChanged(ContextChangedEventArgs& e) {
//
//		// Store the last context given so it can be passed to rooms (likely to be global context only).
//		_context = e.Context();
//	
//		// Update the context for the current room if we already have one loaded.
//		_callRoomOnContextChanged();
//
//	}
//
//	void RoomManager::_loadNextRoom() {
//
//		if (!IsRoomNull()) {
//
//			// If the current room has been set up and isn't persistent, reset it.
//			if (Room()->IsReady() && !_current_room->Persistent())
//				Room()->OnReset();
//
//			// Call the on exit event for the current room.
//			RoomExitEventArgs args;
//			Room()->OnExit(args);
//
//		}
//
//		// Update current room.
//		_current_room = std::move(_next_room);
//		_callRoomOnContextChanged();
//
//		// Call the on enter event for the new room.
//		RoomEnterEventArgs args;
//		Room()->OnEnter(args);
//
//		// If the next room hasn't been set up yet, set it up.
//
//		if (!Room()->IsReady()) {
//
//			RoomCreateEventArgs args;
//			Room()->OnCreate(args);
//
//		}
//
//	}
//	// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
//	void RoomManager::_exitRoom() {
//
//		if (!IsRoomNull()) {
//
//			Room()->OnReset();
//
//			RoomExitEventArgs args;
//			Room()->OnExit(args);
//
//			_current_room.reset();
//
//		}
//
//	}
//	void RoomManager::_callRoomOnContextChanged() const {
//
//		if (!Room())
//			return;
//
//		ContextChangedEventArgs context_changed_args(_context);
//		Room()->OnContextChanged(context_changed_args);
//
//	}
//	bool RoomManager::_roomTransitionIsInProgress() const {
//
//		return _room_transition_state != NO_TRANSITION_PENDING;
//
//	}
//	void RoomManager::_beginRoomTransition(IRoomPtr& next_room) {
//
//		_next_room = next_room;
//		_room_transition_state = TRANSITION_PENDING;
//
//	}
//
//}