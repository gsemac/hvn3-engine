#include "hvn3/sound/Sound.h"
#include <allegro5/allegro_audio.h>

namespace hvn3 {

	Sound::Sound(const char* filename) {

		__sample = al_load_sample(filename);

	}
	Sound::Sound(Sound&& other) {

		__sample = other.__sample;

		other.__sample = nullptr;

	}
	Sound::~Sound() {

		if (__sample)
			al_destroy_sample(__sample);

		__sample = nullptr;

	}

	ALLEGRO_SAMPLE_ID* Sound::Play() {

		ALLEGRO_SAMPLE_ID* sample_id = nullptr;
		al_play_sample(__sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, sample_id);
		return sample_id;

	}
	ALLEGRO_SAMPLE_ID* Sound::Loop() {

		ALLEGRO_SAMPLE_ID* sample_id = nullptr;
		al_play_sample(__sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, sample_id);
		return sample_id;

	}

	ALLEGRO_SAMPLE* Sound::AlPtr() {

		return __sample;

	}

}