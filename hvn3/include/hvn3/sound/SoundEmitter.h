#pragma once
#include "hvn3/core/Positionable2dBase.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/sound/Sound.h"

namespace hvn3 {

	class SoundManager;

	class SoundEmitter :
		public Positionable2dBase<float> {

	public:
		SoundEmitter(SoundManager* manager);

		Handle<SoundInstance> Play(const Sound& sound, bool loop = false);

		float Volume();
		void SetVolume(float value);
		void SetFallOff(float max_dist);
		void SetFallOff(float min_dist, float max_dist, float factor = 1.0f);
		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& value);
		Vector2d& GetVelocity();

	private:
		SoundManager* _manager;
		Vector2d _velocity;
		float _volume;
		float _falloff_min, _falloff_max, _falloff_factor;

	};

}