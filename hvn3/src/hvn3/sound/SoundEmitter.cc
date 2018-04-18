#include "hvn3/sound/SoundEmitter.h"

namespace hvn3 {

	SoundEmitter::SoundEmitter(const PointF& position) :
		Positionable2dBase(position) {
		SetFallOff(0.0f, 100.0f, 1.0f);
	}
	float SoundEmitter::Volume() {
		return _volume;
	}
	void SoundEmitter::SetVolume(float value) {
		_volume = value;
	}
	void SoundEmitter::SetFallOff(float max_dist) {
		_falloff_max = max_dist;
	}
	void SoundEmitter::SetFallOff(float min_dist, float max_dist, float factor) {
		_falloff_min = min_dist;
		_falloff_max = max_dist;
		_falloff_factor = factor;
	}
	const Vector2d& SoundEmitter::Velocity() const {
		return _velocity;
	}
	void SoundEmitter::SetVelocity(const Vector2d& value) {
		_velocity = value;
	}
	Vector2d& SoundEmitter::GetVelocity() {
		return _velocity;
	}

}