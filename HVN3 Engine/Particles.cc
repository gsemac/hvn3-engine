#include <algorithm>
#include <climits>
#include <allegro5\allegro_primitives.h>
#include "Particles.h"
#include "Random.h"
#include "Sprite.h"
#include "Utility.h"

namespace hvn3 {

	ParticleType::ParticleType() {

		__sprite_index = -1;
		__animate = false;
		__stretch = false;
		__random = false;

		__size_min = 0;
		__size_max = 0;
		__size_incr = 0;
		__size_wiggle = 0;

		__xscale = 1.0f;
		__yscale = 1.0f;

		__speed_min = 0.0f;
		__speed_max = 0.0f;
		__speed_incr = 0.0f;
		__speed_wiggle = 0.0f;

		__dir_min = 0.0f;
		__dir_max = 0.0f;
		__dir_incr = 0.0f;
		__dir_wiggle = 0.0f;

		__grav_amount = 0.0f;
		__grav_direction = 0.0f;

		__ang_min = 0.0f;
		__ang_max = 0.0f;
		__ang_incr = 0.0f;
		__ang_wiggle = 0.0f;

		__ang_relative = false;
		__additive = false;

		__life_min = 0;
		__life_max = 0;

	}
	void ParticleType::SetSprite(int sprite_index, bool animate, bool stretch, bool random) {

		__sprite_index = sprite_index;
		__animate = animate;
		__stretch = stretch;
		__random = random;

	}
	void ParticleType::SetSize(int size_min, int size_max, int size_incr, int size_wiggle) {

		__size_min = size_min;
		__size_max = size_max;
		__size_incr = size_incr;
		__size_wiggle = size_wiggle;

	}
	void ParticleType::SetScale(float xscale, float yscale) {

		__xscale = xscale;
		__yscale = yscale;

	}
	void ParticleType::SetSpeed(float speed_min, float speed_max, float speed_incr, float speed_wiggle) {

		__speed_min = speed_min;
		__speed_max = speed_max;
		__speed_incr = speed_incr;
		__speed_wiggle = speed_wiggle;

	}
	void ParticleType::SetSpeed(float speed) {
		SetSpeed(speed, speed, 0.0f, 0.0f);
	}
	void ParticleType::SetDirection(float dir_min, float dir_max, float dir_incr, float dir_wiggle) {

		__dir_min = dir_min;
		__dir_max = dir_max;
		__dir_incr = dir_incr;
		__dir_wiggle = dir_wiggle;

	}
	void ParticleType::SetDirection(float dir) {
		SetDirection(dir, dir, 0.0f, 0.0f);
	}
	void ParticleType::SetGravity(float grav_amount, float grav_direction) {

		__grav_amount = grav_amount;
		__grav_direction = grav_direction;

	}
	void ParticleType::SetOrientation(float ang_min, float ang_max, float ang_incr, float ang_wiggle, bool ang_relative) {

		__ang_min = ang_min;
		__ang_max = ang_max;
		__ang_incr = ang_incr;
		__ang_wiggle = ang_wiggle;
		__ang_relative = ang_relative;

	}
	void ParticleType::SetOrientation(float ang) {
		SetOrientation(ang, ang, 0.0f, 0.0f, false);
	}
	void ParticleType::SetBlend(bool additive) {

		__additive = additive;

	}
	void ParticleType::SetLife(int life) {
		SetLife(life, life);
	}
	void ParticleType::SetLife(int life_min, int life_max) {

		__life_min = life_min;
		__life_max = life_max;

	}

	Emitter::Particle::Particle(float x, float y, const ParticleType& type, unsigned long depth) {

		Reset(x, y, type);
		Depth = depth;

	}
	void Emitter::Particle::Reset(float x, float y, const ParticleType& type) {

		SetXY(x, y);
		__velocity = Vector2d(Random::Float(type.__dir_min, type.__dir_max), Random::Float(type.__speed_min, type.__speed_max));
		__gravity = Vector2d(type.__grav_amount, type.__grav_direction);

		__sprite_index = type.__sprite_index;
		__size = Random::Integer(type.__size_min, type.__size_max);
		__xscale = type.__xscale;
		__yscale = type.__yscale;
		__angle = Random::Float(type.__ang_min, type.__ang_max);
		__additive = type.__additive;
		__life = Random::Integer(type.__life_min, type.__life_max);

		__size_wiggle = type.__size_wiggle;
		__speed_wiggle = type.__speed_wiggle;
		__dir_wiggle = type.__dir_wiggle;
		__ang_wiggle = type.__ang_wiggle;

		__size_incr = type.__size_incr;
		__speed_incr = type.__speed_incr;
		__dir_incr = type.__dir_incr;
		__ang_incr = type.__ang_incr;

		__ang_relative = type.__ang_relative;

		if (!__ang_relative)
			SetImageAngle(Random::Float(type.__ang_min, type.__ang_max));

		if (type.__random)
			SetImageIndex(Random::Integer());
		else
			SetImageIndex(0);

		if (type.__sprite_index != -1 && type.__stretch) {
			unsigned int images = Sprite()->Length();
			SetImageSpeed((float)images / __life);
		}
		else if (type.__animate)
			SetImageSpeed(1.0f);

		__image_index_timer = 0.0f;

	}
	bool Emitter::Particle::IsDead() {

		return __life <= 0;

	}
	void Emitter::Particle::OnUpdate(UpdateEventArgs& e) {

		// Update image index.
		__image_index_timer += (std::fabs)(ImageSpeed());
		if (__image_index_timer >= 1.0f) {
			switch (Sign(ImageSpeed())) {
			case -1:
				if (ImageIndex() == 0)
					SetImageIndex(INT_MAX);
				else
					SetImageIndex(ImageIndex() - 1);
			case 1:
				if (ImageIndex() == INT_MAX)
					SetImageIndex(0);
				else
					SetImageIndex(ImageIndex() + 1);
			}
			__image_index_timer -= 1.0f;
		}

		// Update size.
		__size += __size_incr;
		if (__size_wiggle)
			__size += Random::Integer(-__size_wiggle, __size_wiggle);

		// Update speed (just calculate new speed).
		float new_speed = __velocity.Magnitude() + __speed_incr;
		if (__speed_wiggle)
			new_speed += Random::Float(-__speed_wiggle, __speed_wiggle);

		// Update direction (just calculate new direction).
		float new_direction = __velocity.Angle() + __dir_incr;
		if (__dir_wiggle)
			new_direction += Random::Float(-__dir_wiggle, __dir_wiggle);

		// Update orientation.
		if (__ang_relative) {
			SetImageAngle(new_direction);
		}
		else {
			SetImageAngle(ImageAngle() + __ang_incr);
			if (__dir_wiggle)
				SetImageAngle(ImageAngle() + Random::Float(-__ang_wiggle, __ang_wiggle));
		}

		// Update velocity.
		__velocity = Vector2d(new_direction, (std::max)(0.0f, new_speed));

		// Update position.
		Translate(__velocity.X() + __gravity.X(), __velocity.Y() + __gravity.Y());

		// Update the life of the particle.
		--__life;

	}
	void Emitter::Particle::OnDraw(DrawEventArgs& e) {

		if (Sprite())
			ISpriteable::OnDraw(e);
		else
			e.Graphics().DrawRectangle(X(), Y(), 10.0f, 10.0f, Color::Black, 5.0f);

	}
	bool operator<(const Emitter::Particle& a, const Emitter::Particle& b) {

		return a.Depth < b.Depth;

	}
	bool operator==(const Emitter::Particle& a, const Emitter::Particle& b) {

		return a.Depth == b.Depth;

	}

	Emitter::Emitter() : Emitter(0.0f, 0.0f) {}
	Emitter::Emitter(float x, float y) : IPositionable(x, y) {

		__particle_count = 0;

	}
	void Emitter::Burst(const ParticleType& type, int number) {

		Burst(X(), Y(), type, number);

	}
	void Emitter::Burst(float x, float y, const ParticleType& type, int number) {

		// Get the highest particle depth.
		unsigned long top_depth = __particles.size() > 0 ? __particles[-1 + __particles.size()].Depth : 0;

		// Add particles to the particle list until we have the necessary amount free to work with.
		int total_size = static_cast<int>(__particles.size());
		int usable_particles = total_size - __particle_count;
		for (int i = usable_particles; i < number; ++i)
			__particles.push_back(Particle(x, y, type, ++top_depth));

		// Reset particles for use (only the ones we didn't add).
		for (int i = __particle_count; i < __particle_count + usable_particles; ++i)
			__particles[i].Reset(x, y, type);
		//for (size_t i = __particle_count; i < (std::min)(total_size, number); ++i) {
		//	int ind = __free_indices.front();
		//	__free_indices.pop();
		//	__particles[ind].Reset(type);
		//}

		// Increase the number of particles in use.
		__particle_count += number;

	}
	int Emitter::Count() {

		return __particle_count;

	}
	void Emitter::OnUpdate(UpdateEventArgs& e) {

		// Update the state of all particles.
		int i = 0;
		while (i < __particle_count) {

			Particle& p = __particles[i];

			// Update the particle.
			p.OnUpdate(e);

			// Check if the vector is dead.
			if (p.IsDead()) {
				// If it is, add the index to the free index list and decrease the particle count.
				std::swap(__particles[i], __particles[__particle_count - 1]);
				//__free_indices.push(i);
				--__particle_count;
			}

			++i;

		}
		//std::cout << "__particle_count " << __particle_count << std::endl;
		//std::cout << "Count(): " << Count() << std::endl;

	}
	void Emitter::OnDraw(DrawEventArgs& e) {

		// Draw all particles.
		for (int i = 0; i < __particle_count; ++i)
			__particles[i].OnDraw(e);

	}

}