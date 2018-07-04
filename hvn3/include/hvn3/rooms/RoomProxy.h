#pragma once
#include "hvn3/rooms/RoomEnterEventArgs.h"
#include "hvn3/rooms/RoomExitEventArgs.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	class IRoom;

	namespace System {

		class RoomProxy {

		public:
			RoomProxy(IRoom& room);

			void SetUp();
			bool IsReady() const;
			void Reset();
			void Restart();

			void DispatchEvent(RoomEnterEventArgs& e);
			void DispatchEvent(RoomExitEventArgs& e);

		private:
			IRoom* _room;

		};

	}
}