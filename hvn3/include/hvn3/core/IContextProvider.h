#pragma once
#include "hvn3/core/Context.h"
#include "hvn3/core/ManagerBase.h"

namespace hvn3 {

	class IGameManager;
	class RoomManager;

	namespace System {

		class IContextProvider {
			friend class hvn3::Context;

		public:
			// Returns an object representing the current game state.
			virtual hvn3::Context Context() = 0;

		protected:
			virtual IGameManager& GetGameManager() = 0;
			virtual RoomManager& GetRoomManager() = 0;
			virtual ManagerBase& GetManagerById(ManagerId id) = 0;

		};

	}
}