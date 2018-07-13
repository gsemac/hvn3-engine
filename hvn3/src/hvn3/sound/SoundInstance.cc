#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/sound/SoundInstance.h"
#include <allegro5/allegro_audio.h>

namespace hvn3 {

	SoundInstance::SoundInstance(const Sound& sound) {

		_instance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>(al_create_sample_instance(System::AllegroAdapter::ToSample(sound)), al_destroy_sample_instance);

	}
	void SoundInstance::Play() {

		SetPosition(_position);
		al_set_sample_instance_playing(get(), true);

	}
	bool SoundInstance::IsPlaying() const {

		if (get() == nullptr)
			return false;

		return al_get_sample_instance_playing(get());
	}
	void SoundInstance::Pause() {

		_position = Position();
		al_set_sample_instance_playing(get(), false);

	}
	void SoundInstance::Stop() {

		_position = 0;
		Pause();

	}
	bool SoundInstance::IsLooping() const {
		return al_get_sample_instance_playmode(get()) == ALLEGRO_PLAYMODE_LOOP;
	}
	void SoundInstance::SetLooping(bool value) {
		al_set_sample_instance_playmode(get(), value ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE);
	}
	float SoundInstance::Volume() const {
		return al_get_sample_instance_gain(get());
	}
	void SoundInstance::SetVolume(float value) {

		// Don't allow negative gains; just mute the sound.
		if (value < 0.0f)
			value = 0.0f;

		al_set_sample_instance_gain(get(), value);

	}
	float SoundInstance::Pan() const {
		return al_get_sample_instance_pan(get());
	}
	void SoundInstance::SetPan(float value) {
		al_set_sample_instance_pan(get(), value);
	}
	float SoundInstance::Speed() const {
		return al_get_sample_instance_speed(get());
	}
	void SoundInstance::SetSpeed(float value) {
		al_set_sample_instance_speed(get(), value);
	}
	unsigned int SoundInstance::Position() const {
		return al_get_sample_instance_position(get());
	}
	void SoundInstance::SetPosition(unsigned int position) {
		al_set_sample_instance_position(get(), position);
	}
	float SoundInstance::Length() const {
		return al_get_sample_instance_time(get());
	}
	ALLEGRO_SAMPLE_INSTANCE* SoundInstance::get() const {
		return _instance.get();
	}
	SoundInstance::operator bool() const {
		return static_cast<bool>(_instance);
	}

	SoundInstance::SoundInstance(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>& sample) {

		_instance = sample;
		_position = 0;

	}

}