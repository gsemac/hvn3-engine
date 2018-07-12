#pragma once
#include "hvn3/core/IGameManager.h"
#include "hvn3/core/Properties.h"
#include "hvn3/core/Runner.h"
#include "hvn3/rooms/RoomManager.h"

namespace hvn3 {

	class GameManager : public IGameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);
		GameManager(const System::Properties& properties);
		~GameManager();

		// Inherited from IGameManager

		// Initializes the game and underlying framework.
		void Initialize(int argc, char* argv[]) override;
		// Runs the main game loop.
		void Loop() override;
		// Deinitializes the game and underlying framework.
		void Shutdown() override;

		System::Properties& GetProperties() override;
		System::Runner& GetRunner() override;
		Display& GetDisplay() override;

		// Inherited from IContextProvider

		// Returns an object representing the current game context.
		hvn3::Context Context() override;

	protected:
		IGameManager& GetGameManager() override;
		RoomManager& GetRoomManager() override;
		ManagerBase* GetManager(ManagerId id) override;

	private:
		System::Properties _properties;
		Display _display;
		hvn3::RoomManager _room_manager;
		System::Runner _runner;

		void _onInit();
		void _onShutdown();

	};

}