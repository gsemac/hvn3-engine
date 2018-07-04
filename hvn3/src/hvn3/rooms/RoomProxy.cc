#include "hvn3/rooms/RoomProxy.h"
#include "hvn3/rooms/IRoom.h"

namespace hvn3 {
	namespace System {

		RoomProxy::RoomProxy(IRoom& room) : _room(&room) {}

		void RoomProxy::SetUp() {
			_room->OnCreate();
		}
		bool RoomProxy::IsReady() const {
			return _room->IsReady();
		}
		void RoomProxy::Reset() {
			_room->OnReset();
		}
		void RoomProxy::Restart() {
			_room->Restart();
		}

		void RoomProxy::DispatchEvent(RoomEnterEventArgs& e) {
			_room->OnEnter(e);
		}
		void RoomProxy::DispatchEvent(RoomExitEventArgs& e) {
			_room->OnExit(e);
		}

	}
}