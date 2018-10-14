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

		struct Particle {
			float x;
			float y;
			float dx;
			float dy;
			int life;
		};

	public:
		ParticleSystem() {}
		ParticleSystem(size_t reserve) {

			_particles.reserve(reserve);

		}

		void CreateParticle(const ParticleType& type, float x, float y) {

			Particle p;
			Vector2d direction = Vector2d::FromDirection(Random::Float(type.directionMin, type.directionMax), Random::Float(type.speedMin, type.speedMax));

			p.x = x;
			p.y = y;
			p.dx = direction.X();
			p.dy = direction.Y();
			p.life = Random::UInteger(type.lifeMin, type.lifeMax);

			_particles.push_back(p);

		}

		void OnDraw(DrawEventArgs& e) override {

			for (auto i = _particles.begin(); i != _particles.end(); ++i) {
				// #todo This is just a placeholder for now
				e.Graphics().DrawSolidCircle(i->x, i->y, 3.0f, Color::Yellow);

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

					// If this particle is still alive, swap its position with that of the earliest dead particle.
					// After the loop, all living particles will be at the beginning of the list.

					if (dead_index < _particles.size()) {

						std::swap(_particles[i], _particles[dead_index]);

						// The dead index has moved to this particle's old position.
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

	private:
		std::vector<Particle> _particles;

	};

}