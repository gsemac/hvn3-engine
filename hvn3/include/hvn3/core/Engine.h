#pragma once

#include "hvn3/core/Runner.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {
	namespace System {

		enum class EngineComponent {
			// Used to initialize core engine components, enabling basic use of the engine.
			Core = 1,
			// Used to initialize I/O engine components, enabling mouse and keyboard input.
			IO = 2,
			Graphics = 4,
			Audio = 8,
			// Used to initialize all engine components.
			Full = Core | IO | Graphics | Audio
		};
		ENABLE_BITFLAG_OPERATORS(EngineComponent);

		class Engine {

			typedef unsigned int ref_count_type;

		public:
			Engine() = delete;

			static void Initialize();
			static void Initialize(int argc, char *argv[]);
			static void Deinitialize();

			static bool IsInitialized();
			static bool IsDeinitialized();

			static void RequireComponent(EngineComponent component);
			static void ReleaseComponent(EngineComponent component);

		private:
			static bool _initialized;
			static ref_count_type _core_ref_count;
			static ref_count_type _io_ref_count;
			static ref_count_type _graphics_ref_count;
			static ref_count_type _audio_ref_count;

		};

	}
}