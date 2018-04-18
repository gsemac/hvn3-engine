#pragma once
#include "hvn3/core/Handle.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/sound/Sound.h"
#include "hvn3/sound/SoundEmitter.h"
#include "hvn3/sound/SoundListener.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {

	class SoundManager :
		public IUpdatable {

	public:
		class SoundInstance {
			friend class SoundManager;

		public:
			SoundInstance();

			void Resume();
			void Pause();
			bool Looping();
			void SetLooping(bool value);
			float Volume();
			void SetVolume(float value);
			float Pan();
			void SetPan(float value);
			float Speed();
			void SetSpeed(float value);

			explicit operator bool();

		private:
			ALLEGRO_SAMPLE_INSTANCE* _instance;

			SoundInstance(ALLEGRO_SAMPLE_INSTANCE* sample_instance);
			ALLEGRO_SAMPLE_INSTANCE* _getPtr();

		};

	private:
		typedef unsigned int EmitterId;
		typedef unsigned int SoundInstanceId;

		struct SoundInstanceData {
			EmitterId emitterId;
			SoundInstance soundInstance;
		};

	public:
		SoundManager();

		Handle<SoundInstance> Play(const Sound& sound, bool loop = false);
		Handle<SoundInstance> Play(const Sound& sound, bool loop, float volume, float pan, float speed);
		Handle<SoundInstance> PlayAt(const Sound& sound, SoundEmitter* emitter, bool loop = false);

		void StopAll();
		void Clear();

		float Volume() const;
		void SetVolume(float value);
		SoundListener& GetListener();
		Handle<SoundEmitter> CreateEmitter();
		void DestroyEmitter(SoundEmitter* handle);

		void OnUpdate(UpdateEventArgs& e) override;

		// Sets the maximum number of samples that can be played simultaneously.
		static void SetNumberOfSamples(int samples);

	private:
		SoundListener _listener;
		float _volume;

		EmitterId _next_emitter_id; // Incremented to get the next emitter ID.
		SoundInstanceId _next_sound_id; // Incremented to get the next sound ID.
		std::vector<EmitterId> _free_emitter_ids; // Contains a collection of removed emitter IDs for reuse.
		std::vector<SoundInstanceId> _free_sound_ids; // Contains a collection of removed sound IDs for reuse.
		std::unordered_map<EmitterId, SoundEmitter> _emitters;
		std::unordered_map<SoundInstanceId, SoundInstanceData> _sounds;

		SoundInstance _playSound(const Sound& sound, bool loop, float volume, float pan, float speed);
		SoundInstance _playSoundAtEmitter(const Sound& sound, SoundEmitter* emitter, bool loop);
		EmitterId _getNextEmitterId();
		SoundInstanceId _getNextSoundId();

	};

}