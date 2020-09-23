#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/sound/Sound.h"
#include "hvn3/sound/SoundEmitter.h"
#include "hvn3/sound/SoundInstance.h"
#include "hvn3/sound/SoundListener.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {

	class SoundManager :
		public IUpdatable {

		typedef unsigned int SoundInstanceId;
		
		struct SoundInstanceData {
			
			SoundInstanceData();
			
			SoundInstance instance;
			int emitterKey;
			bool usesInternalEmitter;
			bool pendingRemoval;

		};

		struct SoundEmitterData {
			SoundEmitter emitter;
		};

		typedef unsigned int emitter_key_type;
		typedef std::vector<SoundInstanceData> sound_instance_container_type;

	public:
		SoundManager();

		SoundInstance Play(const Sound& sound, bool loop = false);
		SoundInstance Play(const Sound& sound, bool loop, float volume, float pan, float speed);
		SoundInstance PlayAt(const Sound& sound, int emitter_key, bool loop);
		SoundInstance PlayAt(const Sound& sound, const PointF& position, bool loop);

		void StopAll();
		void Clear();

		float Volume() const;
		void SetVolume(float value);
		SoundListener& GetListener();
		emitter_key_type AddEmitter(const SoundEmitter& emitter);
		SoundEmitter& EmitterAt(emitter_key_type key);
		void RemoveEmitter(emitter_key_type key);

		void OnUpdate(UpdateEventArgs& e) override;

		// Sets the maximum number of samples that can be played simultaneously.
		static void SetNumberOfSamples(int samples);

	private:
		SoundListener _listener;
		float _volume;
		std::unordered_map<emitter_key_type, SoundEmitterData> _emitters;
		std::unordered_map<emitter_key_type, SoundEmitterData> _internal_emitters;
		sound_instance_container_type _sounds;
		emitter_key_type _next_emitter_key;
		emitter_key_type _next_internal_emitter_key;

		emitter_key_type _getNextEmitterKey(bool _internal);
		SoundEmitterData* _getEmitterData(SoundInstanceData& sound_data);
		SoundEmitterData* _getEmitterData(emitter_key_type key, bool _internal);
		void _updateSound(SoundInstanceData& sound_data);
		void _updateSound(SoundInstanceData& sound_data, SoundEmitterData& emitter_data);
		SoundManager::SoundInstanceData* _playSound(const Sound& sound, bool play, bool loop, float volume, float pan, float speed);
		
	};

}