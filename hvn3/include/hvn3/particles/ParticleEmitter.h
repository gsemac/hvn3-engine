#pragma once
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/particles/ParticleDefs.h"
#include "hvn3/particles/ParticleType.h"

namespace hvn3 {

	class ParticleEmitter {

	public:
		ParticleEmitter() {}
		ParticleEmitter(const PointF& position) :
			ParticleEmitter(position.x, position.y) {
		}
		ParticleEmitter(float x, float y) :
			_region(x, y, 0.0f, 0.0f) {
		}

		void SetDistribution(ParticleDistribution value) {
			_distribution = value;
		}

		void Burst(const ParticleType& type) const {



		}

	private:
		RectangleF _region;
		ParticleDistribution _distribution;

	};

}