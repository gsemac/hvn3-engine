#pragma once

#include "hvn3/core/Runner.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3::core {

	enum class EngineModules {
		// The core engine module, enabling primary engine features.
		Core = 1,
		// The I/O engine module, enabling keyboard and mouse input.
		IO = 2,
		Graphics = 4,
		Audio = 8,
		All = Core | IO | Graphics | Audio
	};

	HVN3_ENABLE_BITFLAGS(EngineModules);

	class Engine {

		typedef unsigned int ref_count_type;

	public:
		Engine() = delete;

		static void Initialize();
		static void Initialize(EngineModules modules);

		static void Deinitialize();
		static void Deinitialize(EngineModules modules);

		static bool IsInitialized();
		static bool IsInitialized(EngineModules modules);

		static bool IsUninitialized();
		static bool IsUninitialized(EngineModules modules);

	private:
		static ref_count_type coreRefCount;
		static ref_count_type ioRefCount;
		static ref_count_type graphicsRefCount;
		static ref_count_type audioModuleRefCount;

		static void InitializeCore();
		static void DeinitializeCore();

		static void InitializeIO();
		static void DeinitializeIO();

		static void InitializeGraphics();
		static void DeinitializeGraphics();

		static void InitializeAudio();
		static void DeinitializeAudio();

	};

}