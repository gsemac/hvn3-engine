#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/sound/SoundManager.h"
#include "hvn3/utility/Algorithm.h"
#include <allegro5/allegro_audio.h>
#include <limits>

namespace hvn3 {

	SoundManager::SoundInstance::SoundInstance() {
		_instance = nullptr;
	}
	void SoundManager::SoundInstance::Resume() {
		al_set_sample_instance_playing(_instance, true);
	}
	void SoundManager::SoundInstance::Pause() {
		al_set_sample_instance_playing(_instance, false);
	}
	bool SoundManager::SoundInstance::Looping() {
		return al_get_sample_instance_playmode(_instance) == ALLEGRO_PLAYMODE_LOOP;
	}
	void SoundManager::SoundInstance::SetLooping(bool value) {
		al_set_sample_instance_playmode(_instance, value ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE);
	}
	float SoundManager::SoundInstance::Volume() {
		return al_get_sample_instance_gain(_instance);
	}
	void SoundManager::SoundInstance::SetVolume(float value) {
		al_set_sample_instance_gain(_instance, value);
	}
	float SoundManager::SoundInstance::Pan() {
		return al_get_sample_instance_pan(_instance);
	}
	void SoundManager::SoundInstance::SetPan(float value) {
		al_set_sample_instance_pan(_instance, value);
	}
	float SoundManager::SoundInstance::Speed() {
		al_get_sample_instance_speed(_instance);
	}
	void SoundManager::SoundInstance::SetSpeed(float value) {
		al_set_sample_instance_speed(_instance, value);
	}
	SoundManager::SoundInstance::operator bool() {
		return _instance != nullptr;
	}

	SoundManager::SoundInstance::SoundInstance(ALLEGRO_SAMPLE_INSTANCE* sample_instance) {
		_instance = sample_instance;
	}
	ALLEGRO_SAMPLE_INSTANCE* SoundManager::SoundInstance::_getPtr() {
		return _instance;
	}



	SoundManager::SoundManager() :
		_listener(PointF(0.0f, 0.0f)) {

		_volume = 1.0f;
		_next_emitter_id = 0;
		_next_sound_id = 0;
		
	}
	Handle<SoundManager::SoundInstance> SoundManager::Play(const Sound& sound, bool loop) {
		return Play(sound, loop, 1.0f, 0.0f, 1.0f);
	}
	Handle<SoundManager::SoundInstance> SoundManager::Play(const Sound& sound, bool loop, float volume, float pan, float speed) {

		SoundInstanceData data;
		data.emitterId = -1;
		data.soundInstance = _playSound(sound, loop, volume, pan, speed);



		return Handle<SoundInstance> data.soundInstance;

	}
	Handle<SoundManager::SoundInstance> SoundManager::PlayAt(const Sound& sound, SoundEmitter* emitter, bool loop) {

		SoundInstanceData data;
		data.emitterId = id;
		data.soundInstance = _playSoundOnEmitter(sound, id, loop);

		_sounds.push_back(data);

		return data.soundInstance;

	}
	void SoundManager::StopAll() {

		// Stop all samples that belong to this manager.
		for (auto i = _sounds.begin(); )

	}
	void SoundManager::Clear() {

		StopAll();

	}
	float SoundManager::Volume() const {
		return _volume;
	}
	void SoundManager::SetVolume(float value) {
		_volume = value;
	}
	SoundListener& SoundManager::GetListener() {
		return _listener;
	}
	Handle<SoundEmitter> SoundManager::CreateEmitter() {

	}
	void SoundManager::DestroyEmitter(SoundEmitter* handle) {

	}
	void SoundManager::OnUpdate(UpdateEventArgs& e) {

	}
	void SoundManager::SetNumberOfSamples(int samples) {
		al_reserve_samples(samples);
	}

	SoundManager::SoundInstance SoundManager::_playSound(const Sound& sound, bool loop, float volume, float pan, float speed) {

		// Scale the volume of the sound according to the volume of the manager.
		volume *= _volume;

		SoundInstance sample(al_create_sample_instance(System::AllegroAdapter::ToSample(sound)));

		if (sample) {

			al_attach_sample_instance_to_mixer(sample._getPtr(), al_get_default_mixer());

			sample.SetLooping(loop);
			sample.SetVolume(volume);
			sample.SetPan(pan);
			sample.SetSpeed(speed);

			al_set_sample_instance_playing(sample._getPtr(), true);

		}

		return sample;

	}
	SoundManager::SoundInstance SoundManager::_playSoundAtEmitter(const Sound& sound, SoundEmitter* emitter, bool loop) {



	}
	SoundManager::EmitterId SoundManager::_getNextEmitterId() {
	
		EmitterId next = NextIncrementIf(_next_emitter_id, [this](EmitterId x) {return _emitters.count(x) == 0; }, true);

		if(next == _next_emitter_id)


		while(_emm)

	}
	SoundManager::SoundInstanceId SoundManager::_getNextSoundId() {

	}

}