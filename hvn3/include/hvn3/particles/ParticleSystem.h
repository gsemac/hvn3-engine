#pragma once
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/particles/ParticleType.h"
#include "hvn3/utility/Random.h"

#include <cstddef>
#include <utility>
#include <vector>

namespace hvn3 {

	class ParticleSystem :
		IDrawable,
		IUpdatable {

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
		ParticleSystem() {}
		ParticleSystem(size_t reserve) {

			_particles.reserve(reserve);

		}

		void CreateParticle(const ParticleType& type, float x, float y) {

			if (_max_particles > 0 && _particles.size() >= _max_particles)
				return;

			_particles.push_back(BuildParticle(type, x, y));

		}
		void Reserve(size_t value) {
			_particles.reserve(value);
		}
		void SetMaxParticles(size_t value) {
			_max_particles = value;
		}
		size_t Count() const {
			return _particles.size();
		}
		void Clear() {
			_particles.clear();
		}

		void OnDraw(DrawEventArgs& e) override {

			for (auto i = _particles.begin(); i != _particles.end(); ++i) {

				// #todo This is just a placeholder for now
				e.Graphics().DrawSolidCircle(i->x, i->y, 3.0f, Color::Merge(i->color1, i->color2, static_cast<float>(i->lifeMax - i->life) / i->lifeMax));

			}

		}
		void OnUpdate(UpdateEventArgs& e) override {

			size_t dead_index = _particles.size();
			size_t dead_count = 0;

			for (size_t i = 0; i < _particles.size(); ++i) {

				Particle* p = &_particles[i];

				--(p->life);

				if (p->life > 0) {

					// (Only bother updating living particles.)

					p->x += p->dx;
					p->y += p->dy;
					p->dx += p->ax;
					p->dy += p->ay;

					// If this particle is still alive, swap its position with that of the earliest dead particle.
					// After the loop, all living particles will be at the beginning of the list.

					if (dead_index < _particles.size()) {

						std::swap(_particles[i], _particles[dead_index]);

						// The next dead index will either be the one following the current dead index (if we have a run of dead particles),
						// or the index of the particle we just swapped.
						if (_particles[dead_index + 1].life <= 0)
							++dead_index;
						else
							dead_index = i;

					}

				}
				else if (p->life <= 0) {

					// If this particle is dead, update the index only if this is the first dead particle we've seen since the last swap.
					// Either way, update the dead particle count for resizing the list after the loop.

					if (dead_index >= _particles.size())
						dead_index = i;

					++dead_count;

				}

			}

			if (dead_count > 0)
				_particles.resize(_particles.size() - dead_count);

		}

	protected:
		Particle BuildParticle(const ParticleType& type, float x, float y) {

			Particle p;
			Vector2d direction = Vector2d::FromDirection(Random::Float(type.directionMin, type.directionMax), Random::Float(type.speedMin, type.speedMax));
			Vector2d acceleration = Vector2d::FromDirection(type.gravityDirection, type.gravity);

			p.x = x;
			p.y = y;
			p.dx = direction.X();
			p.dy = direction.Y();
			p.ax = acceleration.X();
			p.ay = acceleration.Y();
			p.life = Random::UInteger(type.lifeMin, type.lifeMax);
			p.lifeMax = type.lifeMax;
			p.color1 = type.colorBegin;
			p.color2 = type.colorEnd;

			return p;

		}

	private:
		std::vector<Particle> _particles;
		size_t _max_particles;

	};

}