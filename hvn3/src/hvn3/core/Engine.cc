#include "hvn3/core/Engine.h"
#include "hvn3/utility/Environment.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <iostream>
#include <memory>
#include <xmmintrin.h>

namespace hvn3 {
	namespace System {

		void Engine::Initialize() {

			assert(!_initialized);

			// Make sure nothing else is already locking components.
			assert(IsDeinitialized());

			RequireComponent(EngineComponent::Full);

			_initialized = true;

		}
		void Engine::Initialize(int argc, char *argv[]) {

			Initialize();

			Environment::SetCommandLineArgs(argc, argv);

		}
		void Engine::Deinitialize() {

			assert(_initialized);

			// Make sure that our initial call to the initialize method is the only thing locking components.
			assert(_core_ref_count == 1);
			assert(_io_ref_count == 1);
			assert(_graphics_ref_count == 1);
			assert(_audio_ref_count == 1);

			ReleaseComponent(EngineComponent::Full);

			_initialized = false;

		}
		bool Engine::IsDeinitialized() {

			return !_initialized &&
				_core_ref_count == 0 &&
				_io_ref_count == 0 &&
				_graphics_ref_count == 0 &&
				_audio_ref_count == 0;

		}
		void Engine::RequireComponent(EngineComponent component) {

			// If the user has manually initialized the engine, do not do any reference counting.
			if (_initialized)
				return;

			// Ensure that core components are always initialized first, in case other components depend on them. 
			if (HasFlag(component, EngineComponent::Core)) {

				if (!al_is_system_installed())
					al_init();

				if (_core_ref_count == 0)
					// Flush denormalized numbers to 0 (performance reasons? I honestly don't remember why I did this).
					_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

				++_core_ref_count;

			}

			if (HasFlag(component, EngineComponent::IO)) {

				if (!al_is_mouse_installed())
					al_install_mouse();

				if (!al_is_keyboard_installed())
					al_install_keyboard();

				++_io_ref_count;

			}

			if (HasFlag(component, EngineComponent::Graphics)) {

				if (_graphics_ref_count == 0) {

					al_init_primitives_addon();
					al_init_font_addon();
					al_init_ttf_addon();
					al_init_image_addon();

				}

				++_graphics_ref_count;

			}

			if (HasFlag(component, EngineComponent::Audio)) {

				if (_audio_ref_count == 0) {

					al_install_audio();
					al_init_acodec_addon();

					// Reserve sound effect sample instances.
					al_reserve_samples(3);

				}

				++_audio_ref_count;

			}

		}
		void Engine::ReleaseComponent(EngineComponent component) {

			// If the user has manually initialized the engine, do not do any reference counting.
			if (_initialized)
				return;

			// Components are deinitialized in the reverse order that they initialized in.

			if (HasFlag(component, EngineComponent::Audio)) {

				assert(_audio_ref_count > 0);

				if (--_audio_ref_count == 0) {

					// There is no uninstall function for al_init_acodec_addon.

					al_install_audio();

				}

			}

			if (HasFlag(component, EngineComponent::Graphics)) {

				assert(_graphics_ref_count > 0);

				if (--_graphics_ref_count == 0) {

					al_shutdown_image_addon();
					al_shutdown_ttf_addon();
					al_shutdown_font_addon();
					al_shutdown_primitives_addon();

				}

			}

			if (HasFlag(component, EngineComponent::IO)) {

				assert(_io_ref_count > 0);

				if (--_io_ref_count == 0) {

					if (!al_is_keyboard_installed())
						al_uninstall_keyboard();

					if (!al_is_mouse_installed())
						al_uninstall_mouse();

				}

			}

			if (HasFlag(component, EngineComponent::Core)) {

				assert(_core_ref_count > 0);

				if (--_core_ref_count == 0) {

					_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);

					if (!al_is_system_installed())
						al_uninstall_system();

				}

			}

		}

		bool Engine::_initialized = false;
		Engine::ref_count_type Engine::_core_ref_count = 0;
		Engine::ref_count_type Engine::_io_ref_count = 0;
		Engine::ref_count_type Engine::_graphics_ref_count = 0;
		Engine::ref_count_type Engine::_audio_ref_count = 0;

	}
}