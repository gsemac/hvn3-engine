#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class RoomCreateEventArgs :
		public System::EventArgs {
	public:
		RoomCreateEventArgs() = default;
	};

	class RoomEnterEventArgs :
		public System::EventArgs {
	public:
		RoomEnterEventArgs() = default;
	};

	class RoomExitEventArgs :
		public System::EventArgs {
	public:
		RoomExitEventArgs() = default;
	};

}