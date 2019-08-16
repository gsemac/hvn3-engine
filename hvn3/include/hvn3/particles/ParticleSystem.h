#pragma once
#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/particles/ParticleEmitter.h"
#include "hvn3/particles/ParticleType.h"

#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

/*

Design considerations:
- Can automatically update emitters
- Provides bounds for culling

*/

namespace hvn3 {

	class ParticleManager;

	class ParticleSystemHandle {

	public:
		ParticleSystemHandle(ParticleManager* manager, ParticleSystemId id);

		ParticleSystem* operator->();
		const ParticleSystem* operator->() const;

	private:
		ParticleManager* _manager;
		ParticleSystemId _id;

	};

	class ParticleSystem :
		public IDrawable,
		public IUpdatable {

	protected:
		struct Particle {
			float x;
			float y;
			float dx;
			float dy;
			float ax;
			float ay;
			Color color1;
			Color color2;
			unsigned int life;
			unsigned int lifeMax;
		};

	public:
		ParticleSystem(ParticleManager* manager);

		void Reserve(size_t value);
		void SetMaxParticles(unsigned int value);

		size_t Count() const;
		void Clear();

		void CreateParticle(ParticleTypeId type, float x, float y);
		ParticleEmitterHandle CreateEmitter();
		ParticleEmitter& GetEmitter(ParticleEmitterId id);
		const ParticleEmitter& GetEmitter(ParticleEmitterId id) const;
		bool EmitterExists(ParticleEmitterId id) const;

		const RectangleF& Bounds() const;

		void OnDraw(DrawEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;

	protected:
		Particle BuildParticle(const ParticleType& type, float x, float y);

	private:
		bool _maxParticleCountReached() const;

		ParticleManager* _manager;
		std::vector<Particle> _particles;
		std::unordered_map<ParticleEmitterId, ParticleEmitter> _emitters;
		unsigned int _max_particles;
		RectangleF _bounds;

	};

}