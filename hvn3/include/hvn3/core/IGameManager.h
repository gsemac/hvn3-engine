#pragma once
#include "hvn3/core/ContextProviderBase.h"

namespace hvn3 {

	class Display;
	class RoomManager;

	namespace System {
		struct Properties;
		class Runner;
	}

	class IGameManager : public System::ContextProviderBase {

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

		virtual System::Properties& GetProperties() = 0;
		virtual System::Runner& GetRunner() = 0;
		virtual Display& GetDisplay() = 0;

	};

}