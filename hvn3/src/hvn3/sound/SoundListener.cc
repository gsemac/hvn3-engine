#include "hvn3/sound/SoundListener.h"

namespace hvn3 {

	SoundListener::SoundListener(const PointF& position) :
		Positionable2dBase(position) {
	}

	const Vector2d& SoundListener::Velocity() const {
		return _velocity;
	}
	void SoundListener::SetVelocity(const Vector2d& value) {
		_velocity = value;
	}
	Vector2d& SoundListener::GetVelocity() {
		return _velocity;
	}

}