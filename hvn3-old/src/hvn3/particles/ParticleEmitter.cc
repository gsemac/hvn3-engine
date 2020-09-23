#include "hvn3/particles/ParticleEmitter.h"
#include "hvn3/particles/ParticleSystem.h"
#include <cassert>

namespace hvn3 {

	ParticleEmitterHandle::ParticleEmitterHandle(ParticleSystem* system, ParticleEmitterId id) {

		_system = system;
		_id = id;

	}
	ParticleEmitter* ParticleEmitterHandle::operator->() {

		assert(_system != nullptr);

		return &_system->GetEmitter(_id);

	}
	const ParticleEmitter* ParticleEmitterHandle::operator->() const {

		assert(_system != nullptr);

		return &_system->GetEmitter(_id);

	}


	ParticleEmitter::ParticleEmitter() {

		_distribution = ParticleDistribution::Linear;
		_system = nullptr;

	}
	ParticleEmitter::ParticleEmitter(ParticleSystem& system) :
		ParticleEmitter() {

		_system = &system;

	}
	const PointF& ParticleEmitter::Position() const {

		return _position;

	}
	void ParticleEmitter::SetPosition(const PointF& position) {

		_position = position;

	}
	void ParticleEmitter::SetPosition(float x, float y) {

		_position = PointF(x, y);

	}
	ParticleDistribution ParticleEmitter::Distribution() const {

		return _distribution;

	}
	void ParticleEmitter::SetDistribution(ParticleDistribution value) {
		_distribution = value;
	}
	//void ParticleEmitter::SetType(ParticleTypeId id) {

	//	_types.clear();

	//	_types.push_back(id);

	//}
	//void ParticleEmitter::SetType(const std::initializer_list<ParticleTypeId>& ids) {

	//	_types.clear();
	//	_types.reserve(ids.size());

	//	for (auto i = ids.begin(); i != ids.end(); ++i)
	//		_types.push_back(*i);

	//}
	void ParticleEmitter::Burst(ParticleTypeId type, unsigned int count) const {

		assert(_system != nullptr);

		for (unsigned int i = 0; i != count; ++i)
			_system->CreateParticle(type, Position().x, Position().y);

	}
	void ParticleEmitter::OnUpdate(UpdateEventArgs& e) {}

}