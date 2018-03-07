#pragma once
#include "hvn3/core/IContextProvider.h"

namespace hvn3 {

	namespace System {
		struct Properties;
		class Runner;
	}

	class RoomManager;

	class IGameManager : public System::IContextProvider {

	public:
		virtual ~IGameManager() = default;

		virtual void Initialize(int argc, char* argv[]) = 0;
		virtual void Loop() = 0;
		virtual void Shutdown() = 0;

		// Returns an object representing the current game state.
		virtual hvn3::Context Context() = 0;

	};

}