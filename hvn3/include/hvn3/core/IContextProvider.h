#pragma once
#include "hvn3/core/ManagerBase.h"

namespace hvn3 {

	class Context;
	class IGameManager;
	class RoomManager;

	namespace System {

		class IContextReceiver;

		class IContextProvider {
			friend class hvn3::Context;

		public:
			// Returns an object representing the current game state.
			virtual hvn3::Context Context() = 0;
			// Provides context to a context receiver via the SetContext method.
			virtual void ProvideContext(IContextReceiver& receiver) = 0;

		protected:
			virtual IGameManager& GetGameManager() = 0;
			virtual RoomManager& GetRoomManager() = 0;
			virtual ManagerBase* GetManager(ManagerId id) = 0;

		};

	}
}