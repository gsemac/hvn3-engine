#pragma once
#include <vector>
#include <memory>
#include "Room.h"
#include "UpdateEventArgs.h"
#include "DrawEventArgs.h"
#include "IRoomManager.h"
#include "RoomTransitionNone.h"
namespace hvn3 {

	class IRoomTransition;

	class RoomManager : public IRoomManager {

		enum ROOM_TRANSITION_STATE : char {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

	public:
		RoomManager();
		~RoomManager();

		void AddRoom(std::unique_ptr<RoomBase>& room);
		void GotoRoom(RoomId id);
		void LoadNext();
		void LoadPrevious();

		void RestartRoom();
		void ClearRoom();
		RoomBase* CurrentRoom();
		size_t CurrentRoomIndex() const;
		size_t RoomCount() const;

		void SetRoomTransition(RoomTransitionPtr& transition);

		void Update(UpdateEventArgs& e);
		void Draw(DrawEventArgs& e);

	protected:
		void LoadRoom(size_t room_index);
		size_t FindRoomIndex(RoomId id) const;
		size_t FindRoomIndex(const std::unique_ptr<RoomBase>& room) const;

		// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
		void ExitCurrentRoom();

		bool RoomTransitionInProgress() const;
		void InitiatePendingRoomTransition(size_t room_index);

	private:
		size_t _current_room;
		size_t _next_room;
		std::vector<std::unique_ptr<RoomBase>> _rooms;
		bool _restart_pending;
		ROOM_TRANSITION_STATE _room_transition_state;
		std::unique_ptr<IRoomTransition> _transition;

	};

}