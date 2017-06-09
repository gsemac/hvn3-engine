#ifndef __PARTICLES_H
#define __PARTICLES_H
#include <vector>
#include <iostream>
#include <queue>
#include "IPositionable.h"
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ISpriteable.h"
#include "Vector2d.h"
#include "UpdateEventArgs.h"
#include "DrawEventArgs.h"

namespace hvn3 {

	enum class ParticleDistribution {
		Linear,
		Gaussian,
		InverseGassian
	};

	class ParticleType {
		friend class Emitter;

	private:
		int __sprite_index;
		bool __animate, __stretch, __random;
		int __size_min, __size_max, __size_incr, __size_wiggle;
		float __xscale, __yscale;
		float __speed_min, __speed_max, __speed_incr, __speed_wiggle;
		float __dir_min, __dir_max, __dir_incr, __dir_wiggle;
		float __grav_amount, __grav_direction;
		float __ang_min, __ang_max, __ang_incr, __ang_wiggle;
		bool __ang_relative;
		bool __additive;
		int __life_min, __life_max;

	public:
		ParticleType();
		void SetSprite(int sprite_index, bool animate, bool stretch, bool random);
		void SetSize(int size_min, int size_max, int size_incr, int size_wiggle);
		void SetScale(float xscale, float yscale);
		void SetSpeed(float speed_min, float speed_max, float speed_incr, float speed_wiggle);
		void SetSpeed(float speed);
		void SetDirection(float dir_min, float dir_max, float dir_incr, float dir_wiggle);
		void SetDirection(float dir);
		void SetGravity(float grav_amount, float grav_direction);
		void SetOrientation(float ang_min, float ang_max, float ang_incr, float ang_wiggle, bool ang_relative);
		void SetOrientation(float ang);
		void SetBlend(bool additive);
		void SetLife(int life);
		void SetLife(int life_min, int life_max);

	};

	class Emitter : public IPositionable, public IUpdatable, public IDrawable {

	private:
		class Particle : public IPositionable, public IUpdatable, public IDrawable, public ISpriteable {

		private:
			int __sprite_index;
			int __size;
			float __xscale;
			float __yscale;
			Vector2d __velocity;
			Vector2d __gravity;
			float __angle;
			bool __ang_relative;
			bool __additive;
			int __life;

			float __image_index_timer;

			int __size_wiggle;
			float __speed_wiggle, __dir_wiggle, __ang_wiggle;

			int __size_incr;
			float __speed_incr, __dir_incr, __ang_incr;

		public:
			unsigned long Depth;

			Particle(float x, float y, const ParticleType& type, unsigned long depth);

			void Reset(float x, float y, const ParticleType& type);
			bool IsDead();

			void OnUpdate(UpdateEventArgs& e) override;
			void OnDraw(DrawEventArgs& e) override;

		};

		ParticleType __type;
		std::vector<Particle> __particles;
		std::queue<int> __free_indices;
		int __particle_count;

	public:
		Emitter();
		Emitter(float x, float y);

		// Create an instantaneous burst of particles.
		void Burst(const ParticleType& type, int number);
		void Burst(float x, float y, const ParticleType& type, int number);

		// Start the emitter particle stream.
		void Stream(const ParticleType& type, int number);

		// Stops the emitter from creating additional particles.
		void Stop();

		void SetRegion(float xmin, float xmax, float ymin, float ymax);
		void SetDistribution(ParticleDistribution distribution);

		int Count();

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

		friend bool operator<(const Particle& a, const Particle& b);
		friend bool operator==(const Particle& a, const Particle& b);

	};

}

#endif