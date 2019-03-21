#include "hvn3/core/Engine.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/io/IOUtils.h"

#include <allegro5/mouse.h>
#include <allegro5/keyboard.h>

#include <fstream>
#include <sstream>

namespace hvn3 {
	namespace io {

		std::string IOUtils::FileToEmbeddableString(const std::string& path) {

			std::ifstream fstream(path, std::ios::in | std::ios::binary);
			std::stringstream byte_string;

			if (!fstream.is_open())
				throw System::IO::FileNotFoundException();

			unsigned char next_byte;
			while (fstream >> next_byte) {
				byte_string << "\\x";
				byte_string << std::hex << (int)next_byte;
			}

			return byte_string.str();

		}

		bool IOUtils::IsPrintableChar(int keyCode) {

			return (keyCode >= 1 && keyCode < 47) || // A-Z, 0-9
				(keyCode >= 60 && keyCode < 63) || // ~ - =
				(keyCode >= 65 && keyCode < 67) || // [ ]
				(keyCode >= 68 && keyCode < 76) || // ; " \ , . / 
				(keyCode >= 86 && keyCode < 89); // / * - + (keypad)

		}
		bool IOUtils::IsPrintableChar(Key key) {
			return IsPrintableChar(static_cast<int>(key));
		}
		bool IOUtils::IsNumeric(hvn3::Key key) {

			int keyCode = static_cast<int>(key);

			return (keyCode >= 27 && keyCode < 47);

		}

		EventSource IOUtils::MouseEventSource() {

			static class ComponentLifetimeHelper {
			public:
				ComponentLifetimeHelper() { System::Engine::RequireComponent(System::EngineComponent::Core | System::EngineComponent::IO); }
				~ComponentLifetimeHelper() { System::Engine::ReleaseComponent(System::EngineComponent::Core | System::EngineComponent::IO); }
			} helper;

			return al_get_mouse_event_source();

		}
		EventSource IOUtils::KeyboardEventSource() {

			static class ComponentLifetimeHelper {
			public:
				ComponentLifetimeHelper() { System::Engine::RequireComponent(System::EngineComponent::Core | System::EngineComponent::IO); }
				~ComponentLifetimeHelper() { System::Engine::ReleaseComponent(System::EngineComponent::Core | System::EngineComponent::IO); }
			} helper;

			return al_get_keyboard_event_source();

		}

	}
}