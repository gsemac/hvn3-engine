#pragma once

struct ALLEGRO_SAMPLE;
struct ALLEGRO_SAMPLE_ID;

namespace hvn3 {

	class Sound {

	private:
		ALLEGRO_SAMPLE* __sample;

	public:
		Sound(const char* filename);
		Sound(Sound&& other);
		~Sound();

		ALLEGRO_SAMPLE_ID* Play();
		ALLEGRO_SAMPLE_ID* Loop();

		ALLEGRO_SAMPLE* AlPtr();

	};

}