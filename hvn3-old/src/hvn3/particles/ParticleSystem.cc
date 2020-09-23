//#include "hvn3/particles/ParticleManager.h"
//#include "hvn3/particles/ParticleSystem.h"
//#include "hvn3/utility/Random.h"
//
//namespace hvn3 {
//
//	ParticleSystemHandle::ParticleSystemHandle(ParticleManager* manager, ParticleSystemId id) {
//
//		_manager = manager;
//		_id = id;
//
//	}
//	ParticleSystem* ParticleSystemHandle::operator->() {
//
//		assert(_manager != nullptr);
//
//		return &_manager->GetSystem(_id);
//
//	}
//	const ParticleSystem* ParticleSystemHandle::operator->() const {
//
//		assert(_manager != nullptr);
//
//		return &_manager->GetSystem(_id);
//
//	}
//
//
//	ParticleSystem::ParticleSystem(ParticleManager* manager) {
//
//		_manager = manager;
//
//	}
//	void ParticleSystem::Reserve(size_t value) {
//		_particles.reserve(value);
//	}
//	void ParticleSystem::SetMaxParticles(unsigned int value) {
//		_max_particles = value;
//	}
//	size_t ParticleSystem::Count() const {
//		return _particles.size();
//	}
//	void ParticleSystem::Clear() {
//		_particles.clear();
//	}
//	void ParticleSystem::CreateParticle(ParticleTypeId type, float x, float y) {
//
//		assert(_manager != nullptr);
//
//		if (_maxParticleCountReached())
//			return;
//
//		_particles.push_back(BuildParticle(_manager->GetType(type), x, y));
//
//	}
//	ParticleEmitterHandle ParticleSystem::CreateEmitter() {
//
//		ParticleSystemId id;
//
//		// Generate a unique ID.
//		while (id = Random::Get<ParticleEmitterId>(), EmitterExists(id));
//
//		_emitters.insert(std::make_pair(id, ParticleEmitter(*this)));
//
//		return ParticleEmitterHandle(this, id);
//
//	}
//	ParticleEmitter& ParticleSystem::GetEmitter(ParticleEmitterId id) {
//
//		assert(EmitterExists(id));
//
//		return _emitters.find(id)->second;
//
//	}
//	const ParticleEmitter& ParticleSystem::GetEmitter(ParticleEmitterId id) const {
//
//		assert(EmitterExists(id));
//
//		return _emitters.find(id)->second;
//
//	}
//	bool ParticleSystem::EmitterExists(ParticleEmitterId id) const {
//
//		return _emitters.count(id) > 0;
//
//	}
//	const RectangleF& ParticleSystem::Bounds() const {
//
//		return _bounds;
//
//	}
//	void ParticleSystem::OnDraw(DrawEventArgs& e) {
//
//		for (auto i = _particles.begin(); i != _particles.end(); ++i) {
//
//			// #todo This is just a placeholder for now
//			e.Graphics().DrawSolidCircle(i->x, i->y, 3.0f, Color::Merge(i->color1, i->color2, static_cast<float>(i->lifeMax - i->life) / i->lifeMax));
//
//		}
//
//	}
//	void ParticleSystem::OnUpdate(UpdateEventArgs& e) {
//
//		// Update all emitters.
//
//		for (auto i = _emitters.begin(); i != _emitters.end(); ++i)
//			i->second.OnUpdate(e);
//
//		// Reset system bounds so it can be recalculated.
//		_bounds.SetSize(0.0f, 0.0f);
//
//		// Update all particles.
//
//		size_t dead_index = _particles.size();
//		size_t dead_count = 0;
//
//		for (size_t i = 0; i < _particles.size(); ++i) {
//
//			Particle* p = &_particles[i];
//
//			--(p->life);
//
//			if (p->life > 0) {
//
//				// Only bother updating living particles.
//
//				p->x += p->dx;
//				p->y += p->dy;
//				p->dx += p->ax;
//				p->dy += p->ay;
//
//				// Update the system bounds.
//				// #todo This currently doesn't take into account the size of the particles.
//
//				_bounds.SetX(Math::Min(_bounds.X(), p->x));
//				_bounds.SetX2(Math::Max(_bounds.X2(), p->x));
//				_bounds.SetY(Math::Min(_bounds.Y(), p->y));
//				_bounds.SetY2(Math::Max(_bounds.Y2(), p->y));
//
//				// If this particle is still alive, swap its position with that of the earliest dead particle.
//				// After the loop, all living particles will be at the beginning of the list.
//
//				if (dead_index < _particles.size()) {
//
//					std::swap(_particles[i], _particles[dead_index]);
//
//					// The next dead index will either be the one following the current dead index (if we have a run of dead particles),
//					// or the index of the particle we just swapped.
//					if (_particles[dead_index + 1].life <= 0)
//						++dead_index;
//					else
//						dead_index = i;
//
//				}
//
//			}
//			else {
//
//				// If this particle is dead, update the index only if this is the first dead particle we've seen since the last swap.
//				// Either way, update the dead particle count for resizing the list after the loop.
//
//				if (dead_index >= _particles.size())
//					dead_index = i;
//
//				++dead_count;
//
//			}
//
//		}
//
//		if (dead_count > 0)
//			_particles.resize(_particles.size() - dead_count);
//
//	}
//
//	ParticleSystem::Particle ParticleSystem::BuildParticle(const ParticleType& type, float x, float y) {
//
//		Particle p;
//		Vector2d direction = Vector2d::FromDirection(Random::Float(type.directionMin, type.directionMax), Random::Float(type.speedMin, type.speedMax));
//		Vector2d acceleration = Vector2d::FromDirection(type.gravityDirection, type.gravity);
//
//		p.x = x;
//		p.y = y;
//		p.dx = direction.X();
//		p.dy = direction.Y();
//		p.ax = acceleration.X();
//		p.ay = acceleration.Y();
//		p.life = Random::UInteger(type.lifeMin, type.lifeMax);
//		p.lifeMax = type.lifeMax;
//		p.color1 = type.colorBegin;
//		p.color2 = type.colorEnd;
//
//		return p;
//
//	}
//
//	bool ParticleSystem::_maxParticleCountReached() const {
//
//		return _max_particles > 0 && _particles.size() >= _max_particles;
//
//	}
//
//}