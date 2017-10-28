#include "core/Framework.h"
#include "utility/Environment.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <xmmintrin.h>
#include <memory>
#include <iostream>

namespace hvn3 {
	namespace System {

		Framework::Framework() {

			Initialize();

		}
		Framework::Framework(int argc, char *argv[]) {

			Initialize(argc, argv);

		}
		Framework::~Framework() {

			Shutdown();

		}

		void Framework::Initialize() {

			// If the framework has already been initialized, do nothing.
			if (_initialized)
				return;

			// Initialize the underlying framework.
			_initializeUnderlyingFramework();

			// Flush denormalized numbers to 0 (performance reasons? I honestly don't remember why I did this).
			_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

			// Since the framework has been initialized, set the indicator flag to true.
			_initialized = true;

		}
		void Framework::Initialize(int argc, char *argv[]) {

			// Run basic initialization routine.
			Initialize();

			// Store the command line parameters so that they can be accessed from anywhere in the program.
			Environment::argc = argc;
			Environment::argv = argv;

		}
		void Framework::Shutdown() {

			// If the framework has not been initialized, do nothing.
			if (!_initialized)
				return;

			// Shut down the underlying framework.
			_shutdownUnderlyingFramework();

			// Since the framework has been shut down, set the indicator flag to false.
			_initialized = false;

		}
		bool Framework::Initialized() {

			return _initialized;

		}

		void Framework::_initializeUnderlyingFramework() {

			// Initialize Allegro.
			al_init();

			// Install I/O add-ons.
			al_install_keyboard();
			al_install_mouse();

			// Install audio add-ons.
			al_install_audio();
			al_init_acodec_addon();

			// Install graphics add-ons.
			al_init_primitives_addon();
			al_init_font_addon();
			al_init_ttf_addon();
			al_init_image_addon();

			// Reserve sound effect sample instances.
			al_reserve_samples(3);

			// Enable bilinear filtering for scaled bitmaps.
			// al_add_new_bitmap_flag(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

		}
		void Framework::_shutdownUnderlyingFramework() {

			// Shut down IO add-ons.
			al_uninstall_keyboard();
			al_uninstall_mouse();

			// Shut down audio add-ons.
			al_uninstall_audio();

			// Shut down other add-ons.
			al_shutdown_ttf_addon(); // If any fonts are not freed, will throw an error.
			al_shutdown_font_addon();
			al_shutdown_primitives_addon();
			al_shutdown_image_addon();

			// Shut down Allegro.
			al_uninstall_system();

		}

		bool Framework::_initialized = false;

	}
}