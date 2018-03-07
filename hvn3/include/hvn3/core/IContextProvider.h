#pragma once
#include "hvn3/core/Context.h"

namespace hvn3 {
	namespace System {

		class IContextProvider {
			friend class hvn3::Context;

		public:
			// Returns an object representing the current game state.
			virtual hvn3::Context Context() = 0;

		protected:
			virtual RoomManager& GetRoomManager() = 0;

		};

	}
}