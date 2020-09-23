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
		SoundEmitter();
		SoundEmitter(const PointF& position);
		SoundEmitter(float x, float y);

		float Volume();
		void SetVolume(float value);
		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& value);
		Vector2d& GetVelocity();
		void SetFallOff(float max_dist);
		void SetFallOff(float dist, float max_dist, float factor = 1.0f);
		// Returns the distance at which fall-off begins.
		float FallOffDistance() const;
		// Returns the distance at which the emitter is inaudible.
		float MaximumDistance() const;
		float FallOffFactor() const;

	private:
		Vector2d _velocity;
		float _volume;
		float _falloff_min, _falloff_max, _falloff_factor;

	};

}