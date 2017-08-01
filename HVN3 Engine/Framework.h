#pragma once
#include "Runner.h"
#define HVN3_INIT() hvn3::System::Framework::Initialize(argc, argv); {
#define HVN3_DEINIT() } hvn3::System::Framework::Shutdown();
#define HVN3_BACKEND_ALLEGRO5

namespace hvn3 {
	namespace System {

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
			static void _initializeUnderlyingFramework();
			static void _shutdownUnderlyingFramework();

			static bool _initialized;

		};

	}
}