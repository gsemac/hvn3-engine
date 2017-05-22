#pragma once
#include "Runner.h"
#define InitializeFramework() hvn3::Framework::Initialize(argc, argv); {
#define ShutdownFramework() } hvn3::Framework::Shutdown();
#define HVN3_BACKEND_ALLEGRO5

namespace hvn3 {

	class Framework {

	public:
		Framework();
		Framework(int argc, char *argv[]);
		~Framework();

		static void Initialize();
		static void Initialize(int argc, char *argv[]);
		static void Shutdown();

		static bool Initialized();

	protected:
		static void InitializeUnderlyingFramework();
		static void ShutdownUnderlyingFramework();

		static bool __initialized;

	};

}