#pragma once
#include "hvn3/core/IManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/rooms/IRoomManager.h"
#include "hvn3/rooms/RoomTransition.h"
#include <vector>

namespace hvn3 {

	class RoomManager :
		public IRoomManager {

		enum ROOM_TRANSITION_STATE {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

	public:
		RoomManager();
		~RoomManager();

		void SetRoom(IRoomPtr& room) override;
		IRoomPtr& Room() override;
		const IRoomPtr& Room() const override;
		void RestartRoom() override;
		void ClearRoom() override;
		bool IsRoomNull() override;

		void SetRoomTransition(RoomTransitionPtr& transition);
		void SetRoomTransition(RoomTransition transition);

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;
		void OnContextChanged(ContextChangedEventArgs& e) override;

	protected:
		void _loadNextRoom();
		// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
		void _exitRoom();
		void _callRoomOnContextChanged() const;

		bool _roomTransitionIsInProgress() const;
		void _beginRoomTransition(IRoomPtr& next_room);

	private:
		IRoomPtr _current_room;
		IRoomPtr _next_room;
		ROOM_TRANSITION_STATE _room_transition_state;
		std::unique_ptr<RoomTransitionBase> _transition;
		Context _context;

	};

}