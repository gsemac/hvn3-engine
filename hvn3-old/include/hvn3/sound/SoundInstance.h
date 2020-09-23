#pragma once
#include <hvn3/allegro/AllegroForwardDeclarations.h>
#include <memory>

namespace hvn3 {

	class Sound;
	class SoundManager;

	class SoundInstance {
		// Needs to be able to be able to access the underlying pointer to attach it to a mixer.
		friend class SoundManager;

	public:
		SoundInstance() = default;
		SoundInstance(const Sound& sound);

		// Plays the sound at its current position.
		void Play();
		bool IsPlaying() const;
		// Pauses the sound at its current position.
		void Pause();
		// Stops the sound and resets its position.
		void Stop();
		bool IsLooping() const;
		void SetLooping(bool value);
		float Volume() const;
		void SetVolume(float value);
		float Pan() const;
		void SetPan(float value);
		float Speed() const;
		void SetSpeed(float value);
		unsigned int Position() const;
		void SetPosition(unsigned int position);
		float Length() const;

		// Returns a pointer to the underlying Allegro object.
		ALLEGRO_SAMPLE_INSTANCE* get() const;

		explicit operator bool() const;

	private:
		std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> _instance;
		unsigned int _position;

		SoundInstance(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>& sample);

	};

}