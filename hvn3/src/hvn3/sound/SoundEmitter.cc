#include "hvn3/sound/SoundEmitter.h"

namespace hvn3 {

	SoundEmitter::SoundEmitter() :
		SoundEmitter(0.0f, 0.0f) {
	}
	SoundEmitter::SoundEmitter(const PointF& position) :
		SoundEmitter(position.x, position.y) {
	}
	SoundEmitter::SoundEmitter(float x, float y) :
		Positionable2dBase(x, y) {
		SetFallOff(0.0f, 100.0f, 1.0f);
	}

	float SoundEmitter::Volume() {
		return _volume;
	}
	void SoundEmitter::SetVolume(float value) {
		_volume = value;
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
	void SoundEmitter::SetFallOff(float max_dist) {
		_falloff_max = max_dist;
	}
	void SoundEmitter::SetFallOff(float dist, float max_dist, float factor) {

		_falloff_min = dist;
		_falloff_max = max_dist;
		_falloff_factor = factor;

	}
	float SoundEmitter::FallOffDistance() const {

		return _falloff_min;

	}
	float SoundEmitter::MaximumDistance() const {

		return _falloff_max;

	}
	float SoundEmitter::FallOffFactor() const {

		return _falloff_factor;

	}

}