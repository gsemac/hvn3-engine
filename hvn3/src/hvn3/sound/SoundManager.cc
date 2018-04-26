#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/sound/SoundManager.h"
#include "hvn3/utility/Algorithm.h"
#include <allegro5/allegro_audio.h>
#include <algorithm>
#include <limits>
#include <memory>
#define NULL_EMITTER_KEY 0

namespace hvn3 {

	SoundManager::SoundManager() :
		_listener(PointF(0.0f, 0.0f)) {

		_volume = 1.0f;
		_next_emitter_key = NULL_EMITTER_KEY;
		_next_internal_emitter_key = NULL_EMITTER_KEY;

	}
	SoundInstance SoundManager::Play(const Sound& sound, bool loop) {
		return Play(sound, loop, 1.0f, 0.0f, 1.0f);
	}
	SoundInstance SoundManager::Play(const Sound& sound, bool loop, float volume, float pan, float speed) {

		return _playSound(sound, true, loop, volume, pan, speed)->instance;

	}
	SoundInstance SoundManager::PlayAt(const Sound& sound, int emitter_key, bool loop) {

		SoundEmitter& emitter = EmitterAt(emitter_key);

		SoundInstanceData* instance = _playSound(sound, false, loop, 1.0f, 0.0f, 1.0f);
		instance->emitterKey = emitter_key;

		_updateSound(*instance);
		instance->instance.Play();

		return instance->instance;

	}
	SoundInstance SoundManager::PlayAt(const Sound& sound, const PointF& position, bool loop) {

		// Create a new internal emitter.
		SoundEmitterData data;
		data.emitter.SetPosition(position);

		// Add the new emitter.
		emitter_key_type key = _getNextEmitterKey(true);
		_internal_emitters[key] = data;

		// Create a sound instance, set its state with respect to the emitter, and play it.
		// The sound will be updated continually in the future when the manager is updated.
		SoundInstanceData* instance = _playSound(sound, false, loop, 1.0f, 0.0f, 1.0f);

		instance->usesInternalEmitter = true;
		instance->emitterKey = key;

		_updateSound(*instance);
		instance->instance.Play();

		return instance->instance;

	}
	void SoundManager::StopAll() {

		// Stop all sound instances that belong to this manager.
		for (auto i = _sounds.begin(); i != _sounds.end(); ++i)
			i->instance.Stop();

	}
	void SoundManager::Clear() {

		// Stops all sound instances and frees the data associated with them.
		// No existing sound instances should be accessed after this point.
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
	SoundManager::emitter_key_type SoundManager::AddEmitter(const SoundEmitter& emitter) {

		SoundEmitterData data;
		data.emitter = emitter;

		emitter_key_type key = _getNextEmitterKey(false);
		_emitters[key] = data;

		return key;

	}
	SoundEmitter& SoundManager::EmitterAt(emitter_key_type key) {

		auto it = _emitters.find(key);

		if (it == _emitters.end())
			throw System::NullReferenceException("No sound emitter exists with this key.");

		return it->second.emitter;

	}
	void SoundManager::RemoveEmitter(emitter_key_type key) {
		_emitters.erase(key);
	}
	void SoundManager::OnUpdate(UpdateEventArgs& e) {

		bool pending_removals = false;

		// Update the parameters of all sounds.
		for (auto i = _sounds.begin(); i != _sounds.end(); ++i) {

			_updateSound(*i);

			if (i->pendingRemoval)
				pending_removals = true;

		}

		// If any sounds are pending removal, remove them now.
		if (pending_removals) {
			_sounds.erase(std::remove_if(_sounds.begin(), _sounds.end(), [](const SoundInstanceData& data) {
				return data.pendingRemoval;
			}), _sounds.end());
		}

	}
	void SoundManager::SetNumberOfSamples(int samples) {
		al_reserve_samples(samples);
	}

	SoundManager::emitter_key_type SoundManager::_getNextEmitterKey(bool _internal) {

		emitter_key_type next_key;
		bool is_unique = true;

		if (_internal) {
			next_key = NextIncrementIf(_next_internal_emitter_key, true, [this](emitter_key_type x) { return x != NULL_EMITTER_KEY && _internal_emitters.count(x) == 0; });
			is_unique = !(next_key == _next_internal_emitter_key);
		}
		else {
			next_key = NextIncrementIf(_next_emitter_key, true, [this](emitter_key_type x) { return x != NULL_EMITTER_KEY && _emitters.count(x) == 0; });
			is_unique = !(next_key == _next_emitter_key);
		}

		// If a unique key could not be generated, throw an exception.
		if (!is_unique)
			throw System::Exception("The maximum number of sound emitters has been reached.");

		// Update the value used for generating the next key.
		if (_internal)
			_next_internal_emitter_key = next_key;
		else
			_next_emitter_key = next_key;

		return next_key;

	}
	SoundManager::SoundEmitterData* SoundManager::_getEmitterData(SoundInstanceData& sound_data) {
		return _getEmitterData(sound_data.emitterKey, sound_data.usesInternalEmitter);
	}
	SoundManager::SoundEmitterData* SoundManager::_getEmitterData(emitter_key_type key, bool _internal) {

		SoundEmitterData* emitter_data = nullptr;

		if (_internal) {

			auto it = _internal_emitters.find(key);

			if (it != _internal_emitters.end())
				emitter_data = &it->second;

		}
		else {

			auto it = _emitters.find(key);

			if (it != _emitters.end())
				emitter_data = &it->second;

		}

		return emitter_data;

	}
	void SoundManager::_updateSound(SoundInstanceData& sound_data) {

		if (sound_data.emitterKey == NULL_EMITTER_KEY) {

			// If the sound is not linked to an emitter, no longer playing, and doesn't have any other references, tag it for removal.
			if (!sound_data.instance.IsPlaying() && sound_data.instance._instance.use_count() <= 1)
				sound_data.pendingRemoval = true;

			return;

		}

		// Find the emitter associated with this sound.
		SoundEmitterData* emitter_data = _getEmitterData(sound_data);

		// If the emitter the sound is associated with no longer exists, it should be stopped and removed.
		if (emitter_data == nullptr) {

			sound_data.instance.Stop();
			sound_data.pendingRemoval = true;

			return;

		}

		// Update the sound according to the state of the emitter.
		_updateSound(sound_data, *emitter_data);

	}
	void SoundManager::_updateSound(SoundInstanceData& sound_data, SoundEmitterData& emitter_data) {

		// Update the state of the sound according to the emitter (currently uses linear model).
		// A description of some different models can be found here:
		//	https://docs.yoyogames.com/source/dadiospice/002_reference/game%20assets/sounds/audio_falloff_set_model.html

		float actual_distance = Math::Geometry::PointDistance(emitter_data.emitter.Position(), GetListener().Position());

		float distance = (std::min)(actual_distance, emitter_data.emitter.MaximumDistance());
		float ref_distance = emitter_data.emitter.FallOffDistance();
		float max_distance = emitter_data.emitter.MaximumDistance();

		float gain = 1.0f - emitter_data.emitter.FallOffFactor() * (distance - ref_distance) / (max_distance - ref_distance);

		sound_data.instance.SetVolume(gain);

	}
	SoundManager::SoundInstanceData* SoundManager::_playSound(const Sound& sound, bool play, bool loop, float volume, float pan, float speed) {

		// Scale the volume according to that of the manager.
		volume *= Volume();

		// Create a new sample instance, and attach it to the default mixer so it can be played.
		SoundInstance instance(sound);

		if (!instance)
			throw System::Exception("The sound instance could not be created.");

		al_attach_sample_instance_to_mixer(instance.get(), al_get_default_mixer());

		instance.SetLooping(loop);
		instance.SetVolume(volume);
		instance.SetPan(pan);
		instance.SetSpeed(speed);

		if (play)
			instance.Play();

		// Add the new instance to the collection of sound instances.
		_sounds.push_back(SoundInstanceData());
		_sounds.back().instance = instance;

		return &_sounds.back();

	}



	SoundManager::SoundInstanceData::SoundInstanceData() {

		emitterKey = NULL_EMITTER_KEY;
		usesInternalEmitter = false;
		pendingRemoval = false;

	}

}