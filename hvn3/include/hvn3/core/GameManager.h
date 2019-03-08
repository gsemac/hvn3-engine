#pragma once
#include "hvn3/core/IGameManager.h"
#include "hvn3/core/ApplicationProperties.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/core/ManagerRegistry.h"
#include "hvn3/core/Runner.h"
#include "hvn3/rooms/RoomManager.h"

#include <memory>

namespace hvn3 {

	class GameManager :
		public IGameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);
		GameManager(const ApplicationProperties& properties);
		GameManager(int argc, char* argv[], const ApplicationProperties& properties);
		~GameManager();

		// Inherited from IGameManager

		// Initializes the game and underlying framework.
		void Initialize(int argc, char* argv[]) override;
		// Runs the main game loop.
		void Loop() override;
		// Terminates the main game loop.
		void Exit() override;
		// Deinitializes the game and underlying framework.
		void Shutdown() override;

		ApplicationProperties& Properties() override;
		System::Runner& Runner() override;
		class Display& Display() override;

		// Inherited from IContextProvider

		// Returns an object representing the current game context.
		hvn3::ApplicationContext Context() override;

	private:
		ManagerRegistry _global;
		ApplicationProperties _properties;
		std::unique_ptr<class Display> _display;
		std::unique_ptr<System::Runner> _runner;

		void _onInit();
		void _onShutdown();

	};

}