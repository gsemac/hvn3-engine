#pragma once
#include "hvn3/core/IGameManager.h"
#include "hvn3/core/Properties.h"
#include "hvn3/rooms/RoomManager.h"

namespace hvn3 {

	class GameManager : public IGameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);
		~GameManager();

		void Initialize(int argc, char* argv[]) override;
		void Loop() override;
		void Shutdown() override;

		System::Properties& GetGameProperties() override;
		System::Runner& GetRunner();

		hvn3::Context Context() override;

	protected:
		hvn3::IGameManager& GetGameManager() override;
		hvn3::RoomManager& GetRoomManager() override;

	private:
		System::Properties _properties;
		System::Runner* _runner;
		hvn3::RoomManager _room_manager;

		void _onShutdown();

	};

}