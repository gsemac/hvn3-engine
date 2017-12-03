#pragma once
#include "hvn3/rooms/RoomEnterEventArgs.h"
#include "hvn3/rooms/RoomExitEventArgs.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	class IRoom;

	namespace System {

		class RoomController {

		public:
			RoomController(IRoom* room);

			void SetUp();
			bool IsSetUp() const;
			void Reset();

			void CallRoomEnterEvent(RoomEnterEventArgs& e);
			void CallRoomExitEvent(RoomExitEventArgs& e);

		private:
			IRoom* _room;

		};

	}
}