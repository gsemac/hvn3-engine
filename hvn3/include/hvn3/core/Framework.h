#pragma once
#include "hvn3/core/Runner.h"
#define HVN3_INIT() hvn3::System::Framework::Initialize(argc, argv); {
#define HVN3_DEINIT() } hvn3::System::Framework::Shutdown();

namespace hvn3 {
	namespace System {

		class Framework {

		public:
			// Constructs an object whose lifetime manages the lifetime of the engine framework.
			Framework();
			// Constructs an object whose lifetime manages the lifetime of the engine framework.
			Framework(int argc, char *argv[]);
			// The engine framework is deinitialized on destruction of the object.
			~Framework();
			
			// Initializes the engine framework if it hasn't already been initialized. Otherwise, does nothing.
			// This should be called before interacting with most other engine components.
			static void Initialize();
			
			// Initializes the engine framework if it hasn't already been initialized and sets global argument variables. Otherwise, does nothing.
			// This should be called before interacting with most other engine components.
			static void Initialize(int argc, char *argv[]);
			
			// Deinitializes the engine framework. It is not required that the engine framework be deinitialized.
			// This should only be called after all objects and assets have been freed.
			static void Shutdown();

			// Returns true if the engine framework has been initialized, false otherwise.
			static bool Initialized();

		protected:
			// Initializes the underlying framework/library (Allegro 5).
			static void _initializeUnderlyingFramework();
			// Deinitializes the underlying framework/library (Allegro 5).
			static void _shutdownUnderlyingFramework();

			static bool _initialized;

		};

	}
}