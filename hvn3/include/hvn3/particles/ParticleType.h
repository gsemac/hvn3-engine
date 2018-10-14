#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/utility/Direction8.h"

namespace hvn3 {

	class ParticleType {

		friend class ParticleSystem;

	public:
		ParticleType() {

			SetDirection(0.0f, 360.0f);
			SetLife(10);
			SetSpeed(1.0f);

		}

		void SetDirection(float value) {
			SetDirection(value, value);
		}
		void SetDirection(float min, float max, float delta = 0.0f, float wiggle = 0.0f) {

			directionMin = min;
			directionMax = max;
			directionDelta = delta;
			directionWiggle = wiggle;

		}
		void SetSpeed(float value) {
			SetSpeed(value, value);
		}
		void SetSpeed(float min, float max, float delta = 0.0f, float wiggle = 0.0f) {

			speedMin = min;
			speedMax = max;
			speedDelta = delta;
			speedWiggle = wiggle;

		}
		void SetLife(unsigned int value) {
			SetLife(value, value);
		}
		void SetLife(unsigned int min, unsigned int max) {

			lifeMin = min;
			lifeMax = max;

		}

		float directionMin;
		float directionMax;
		float directionDelta;
		float directionWiggle;
		unsigned int lifeMin;
		unsigned int lifeMax;
		float speedMin;
		float speedMax;
		float speedDelta;
		float speedWiggle;

	};

}