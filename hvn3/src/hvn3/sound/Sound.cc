#include "hvn3/sound/Sound.h"
#include <allegro5/allegro_audio.h>

namespace hvn3 {

	Sound::Sound() {}
	Sound::Sound(const Sound& other) {
		_copyCopy(other);
	}
	Sound::Sound(Sound&& other) {
		_moveCopy(std::move(other));
	}
	unsigned int Sound::Frequency() const {

		if (!_sample)
			return 0;

		al_get_sample_frequency(_getPtr());

	}
	unsigned int Sound::Samples() const {

		if (!_sample)
			return 0;

		al_get_sample_length(_getPtr());

	}
	float Sound::Length() const {

		if (!_sample)
			return 0.0f;

		return static_cast<float>(Frequency()) / Samples();

	}
	Sound Sound::FromFile(const std::string& filename) {

		Sound sample;
		sample._sample = std::shared_ptr<ALLEGRO_SAMPLE>(al_load_sample(filename.c_str()), al_destroy_sample);

		return sample;

	}
	Sound& Sound::operator=(Sound&& other) {
		
		_moveCopy(std::move(other));

		return *this;

	}
	Sound& Sound::operator=(const Sound& other) {
		
		_copyCopy(other);

		return *this;

	}
	
	void Sound::_moveCopy(Sound&& other) {
		_sample = std::move(other._sample);
	}
	void Sound::_copyCopy(const Sound& other) {
		_sample = other._sample;
	}
	ALLEGRO_SAMPLE* Sound::_getPtr() const {

		return _sample.get();

	}

}