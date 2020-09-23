#pragma once
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include <memory>
#include <string>

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}
	
	class Sound {
		friend class System::AllegroAdapter;

	public:
		Sound();
		Sound(const Sound& other);
		Sound(Sound&& other);

		// Returns the frequency of the sample in Hz.
		unsigned int Frequency() const;
		// Returns the number of sample values in the sample.
		unsigned int Samples() const;
		// Returns the length of the sample in seconds.
		float Length() const;

		static Sound FromFile(const std::string& filename);

		Sound& operator=(Sound&& other);
		Sound& operator=(const Sound& other);

	private:
		std::shared_ptr<ALLEGRO_SAMPLE> _sample;

		void _moveCopy(Sound&& other);
		void _copyCopy(const Sound& other);
		ALLEGRO_SAMPLE* _getPtr() const;

	};

}