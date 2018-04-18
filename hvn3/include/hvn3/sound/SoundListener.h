#pragma once
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/core/Positionable2dBase.h"

namespace hvn3 {

	class SoundListener : 
		public Positionable2dBase<float> {

	public:
		SoundListener(const PointF& position);

		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& value);
		Vector2d& GetVelocity();

	private:
		Vector2d _velocity;

	};

}