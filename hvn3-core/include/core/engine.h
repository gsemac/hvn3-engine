#pragma once

#include "core/engine_modules.h"

namespace hvn3::core {

	class Engine {

		typedef unsigned int ref_count_t;

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
		static ref_count_t coreRefCount;
		static ref_count_t ioRefCount;
		static ref_count_t graphicsRefCount;
		static ref_count_t audioModuleRefCount;

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