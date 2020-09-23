#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/particles/ParticleDefs.h"
#include "hvn3/particles/ParticleType.h"

namespace hvn3 {

	class ParticleEmitter;
	class ParticleSystem;

	class ParticleEmitterHandle {

	public:
		ParticleEmitterHandle(ParticleSystem* system, ParticleEmitterId id);

		ParticleEmitter* operator->();
		const ParticleEmitter* operator->() const;

	private:
		ParticleSystem* _system;
		ParticleEmitterId _id;

	};

	class ParticleEmitter :
		public IUpdatable {

	public:
		ParticleEmitter();
		ParticleEmitter(ParticleSystem& system);

		const PointF& Position() const;
		void SetPosition(const PointF& position);
		void SetPosition(float x, float y);

		ParticleDistribution Distribution() const;
		void SetDistribution(ParticleDistribution value);

		//void SetType(ParticleTypeId id);
		//void SetType(const std::initializer_list<ParticleTypeId>& ids);

		void Burst(ParticleTypeId type, unsigned int count) const;

		void OnUpdate(UpdateEventArgs& e) override;

	private:
		ParticleSystem* _system;
		PointF _position;
		RectangleF _region;
		ParticleDistribution _distribution;

	};

}