#pragma once
#include "hvn3/core/IApplicationContextProvider.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"

namespace hvn3 {

	class Display;
	class ApplicationProperties;
	class RoomManager;

	namespace System {
		class Runner;
	}

	class IGameManager :
		public IManager,
		public IContextProvider {

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

		virtual ApplicationProperties& Properties() = 0;
		virtual System::Runner& Runner() = 0;
		virtual Display& Display() = 0;

		virtual hvn3::ApplicationContext Context() = 0;

	};

	template<>
	struct ManagerIdTraits<GAME_MANAGER> {
		typedef IGameManager type;
	};

}