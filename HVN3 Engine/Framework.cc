#include "Framework.h"
#include "Directory.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <xmmintrin.h>
#include <memory>

void Framework::Initialize() {

	// If the framework has already been initialized, do nothing.
	if (__initialized)
		return;

	// Initialize the underlying framework.
	InitializeUnderlyingFramework();

	// Flush denormalized numbers to 0 (performance reasons? I honestly don't remember why I did this).
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

	// Since the framework has been initialized, set the indicator flag to true.
	__initialized = true;

}
void Framework::Initialize(int argc, char *argv[]) {

	// Todo: Store the command line parameters so they can be accessed later from anywhere in the program.

}
void Framework::Shutdown() {

	// If the framework has not been initialized, do nothing.
	if (!__initialized)
		return;

	// Shut down the underlying framework.
	ShutdownUnderlyingFramework();

	// Since the framework has been shut down, set the indicator flag to false.
	__initialized = false;

}

void Framework::InitializeUnderlyingFramework() {

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

}
void Framework::ShutdownUnderlyingFramework() {
	
	try {

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
	catch (...) {

		// It's okay for this to fail, as it would typically be called at the end of the program.
		
	}

}

bool Framework::__initialized = false;