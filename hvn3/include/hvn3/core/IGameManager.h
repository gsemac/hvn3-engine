#pragma once

namespace hvn3 {

	namespace System {
		struct Properties;
		class Runner;
	}

	class RoomManager;

	class IGameManager {

	public:
		virtual ~IGameManager() = 0;

		virtual void Initialize(int argc, char* argv[]) = 0;
		virtual void Loop() = 0;
		virtual void Shutdown() = 0;

		virtual System::Properties& Properties() = 0;
		virtual System::Runner& Runner() = 0;
		virtual RoomManager& Rooms() = 0;

	};

}