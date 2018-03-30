#pragma once
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {
	namespace System {

		typedef unsigned int ManagerId;

		enum BUILTIN_MANAGER_ID : ManagerId {

			GAME_MANAGER,
			ROOM_MANAGER,
			OBJECT_MANAGER,
			COLLISION_MANAGER,
			PHYSICS_MANAGER,

			BASIC_LIGHTING_MANAGER,

			MINIMUM_USER_MANAGER_ID

		};

		class ManagerBase {
		public:
			static ManagerId Id() {
				// Child classes MUST provide their own version of this function and return a unique ID.
				// If this version is being called, you've done something wrong.
				throw System::NotImplementedException("Each manager must return a unique ID by hiding the static Id method.");
			}

		};

	}
}