#pragma once
#include "hvn3/core/ContextProviderBase.h"
#include "hvn3/core/ManagerDefs.h"

namespace hvn3 {

	class Display;
	class GameProperties;
	class RoomManager;

	namespace System {
		class Runner;
	}

	class IGameManager :
		public IManager,
		public System::ContextProviderBase {

	public:
		virtual ~IGameManager() = default;

		// Initializes the game and underlying framework.
		virtual void Initialize(int argc, char* argv[]) = 0;
		// Runs the main game loop.
		virtual void Loop() = 0;
		// Terminates the main game loop.
		virtual void Exit() = 0;
		// Deinitializes the game and underlying framework.
		virtual void Shutdown() = 0;

		virtual GameProperties& GetProperties() = 0;
		virtual System::Runner& GetRunner() = 0;
		virtual Display& GetDisplay() = 0;

	};

	template<>
	struct ManagerTraits<GAME_MANAGER> {
		typedef IGameManager type;
	};

}