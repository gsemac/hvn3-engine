#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class RoomCreateEventArgs :
		public EventArgs {
	public:
		RoomCreateEventArgs() = default;
	};

	class RoomEnterEventArgs :
		public EventArgs {
	public:
		RoomEnterEventArgs() = default;
	};

	class RoomExitEventArgs :
		public EventArgs {
	public:
		RoomExitEventArgs() = default;
	};

}