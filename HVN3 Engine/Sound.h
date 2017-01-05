#ifndef __SOUND_H
#define __SOUND_H
#include <allegro5/allegro_audio.h>

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

#endif