#include "core/engine.h"

#include <cassert>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

namespace hvn3::core {

	// Public members

	void Engine::Initialize() {

		Initialize(EngineModules::All);

	}
	void Engine::Initialize(EngineModules modules) {

		if (HasFlag(modules, EngineModules::Core) && ++coreRefCount <= 1)
			InitializeCore();

		if (HasFlag(modules, EngineModules::IO) && ++ioRefCount <= 1)
			InitializeIO();

		if (HasFlag(modules, EngineModules::Graphics) && ++graphicsRefCount <= 1)
			InitializeGraphics();

		if (HasFlag(modules, EngineModules::Audio) && ++audioModuleRefCount <= 1)
			InitializeAudio();

	}

	void Engine::Deinitialize() {

		Deinitialize(EngineModules::All);

	}
	void Engine::Deinitialize(EngineModules modules) {

		if (HasFlag(modules, EngineModules::Core) && --coreRefCount <= 0)
			DeinitializeCore();

		if (HasFlag(modules, EngineModules::IO) && --ioRefCount <= 0)
			DeinitializeIO();

		if (HasFlag(modules, EngineModules::Graphics) && --graphicsRefCount <= 0)
			DeinitializeGraphics();

		if (HasFlag(modules, EngineModules::Audio) && --audioModuleRefCount <= 0)
			DeinitializeAudio();

	}

	bool Engine::IsInitialized() {

		return IsInitialized(EngineModules::All);

	}
	bool Engine::IsInitialized(EngineModules modules) {

		bool result = true;

		if (HasFlag(modules, EngineModules::Core))
			result &= coreRefCount > 0;

		if (HasFlag(modules, EngineModules::IO))
			result &= ioRefCount > 0;

		if (HasFlag(modules, EngineModules::Graphics))
			result &= graphicsRefCount > 0;

		if (HasFlag(modules, EngineModules::Audio))
			result &= audioModuleRefCount > 0;

		return result;

	}

	bool Engine::IsUninitialized() {

		return IsUninitialized(EngineModules::All);

	}
	bool Engine::IsUninitialized(EngineModules modules) {

		bool result = true;

		if (HasFlag(modules, EngineModules::Core))
			result &= coreRefCount <= 0;

		if (HasFlag(modules, EngineModules::IO))
			result &= ioRefCount <= 0;

		if (HasFlag(modules, EngineModules::Graphics))
			result &= graphicsRefCount <= 0;

		if (HasFlag(modules, EngineModules::Audio))
			result &= audioModuleRefCount <= 0;

		return result;

	}

	// Private members

	Engine::ref_count_t Engine::coreRefCount = 0;
	Engine::ref_count_t Engine::ioRefCount = 0;
	Engine::ref_count_t Engine::graphicsRefCount = 0;
	Engine::ref_count_t Engine::audioModuleRefCount = 0;

	void Engine::InitializeCore() {

		assert(coreRefCount <= 1);

		al_init();

	}
	void Engine::DeinitializeCore() {

		assert(coreRefCount <= 0);

		al_uninstall_system();

	}

	void Engine::InitializeIO() {

		assert(ioRefCount <= 1);

		al_install_mouse();
		al_install_keyboard();

	}
	void Engine::DeinitializeIO() {

		assert(ioRefCount <= 0);

		al_uninstall_keyboard();
		al_uninstall_mouse();

	}

	void Engine::InitializeGraphics() {

		assert(graphicsRefCount <= 1);

		al_init_primitives_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_init_image_addon();

	}
	void Engine::DeinitializeGraphics() {

		assert(graphicsRefCount <= 0);

		al_shutdown_image_addon();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();
		al_shutdown_primitives_addon();

	}

	void Engine::InitializeAudio() {

		assert(audioModuleRefCount <= 1);

		al_install_audio();
		al_init_acodec_addon();

		// Reserve sound effect sample instances.

		al_reserve_samples(3);

	}
	void Engine::DeinitializeAudio() {

		assert(audioModuleRefCount <= 0);

		// There is no uninstall function for al_init_acodec_addon.

		al_uninstall_audio();

	}

}