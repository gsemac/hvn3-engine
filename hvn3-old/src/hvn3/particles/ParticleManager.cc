//#include "hvn3/particles/ParticleManager.h"
//
//namespace hvn3 {
//
//	ParticleManager::ParticleManager() :
//		_default_system(this) {
//	}
//	void ParticleManager::RegisterType(ParticleTypeId id, const ParticleType& type) {
//
//		_types.insert(std::make_pair(id, type));
//
//	}
//	ParticleType& ParticleManager::GetType(ParticleTypeId id) {
//
//		HVN3_ASSERT(TypeIsRegistered(id), "No particle type is registered with the given ID.");
//
//		return _types.find(id)->second;
//
//	}
//	const ParticleType& ParticleManager::GetType(ParticleTypeId id) const {
//
//		HVN3_ASSERT(TypeIsRegistered(id), "No particle type is registered with the given ID.");
//
//		return _types.find(id)->second;
//
//	}
//	bool ParticleManager::TypeIsRegistered(ParticleTypeId id) const {
//
//		return _types.count(id) > 0;
//
//	}
//	ParticleSystem& ParticleManager::DefaultSystem() {
//		return _default_system;
//	}
//	const ParticleSystem& ParticleManager::DefaultSystem() const {
//		return _default_system;
//	}
//	ParticleSystemHandle ParticleManager::CreateSystem() {
//
//		ParticleSystemId id;
//
//		// Generate a unique ID.
//		while (id = Random::Get<ParticleSystemId>(), SystemExists(id));
//
//		_systems.insert(std::make_pair(id, ParticleSystem(this)));
//
//		return ParticleSystemHandle(this, id);
//
//	}
//	ParticleSystem& ParticleManager::GetSystem(ParticleSystemId id) {
//
//		HVN3_ASSERT(SystemExists(id), "No particle system exists with the given ID.");
//
//		return _systems.find(id)->second;
//
//	}
//	const ParticleSystem& ParticleManager::GetSystem(ParticleSystemId id) const {
//
//		HVN3_ASSERT(SystemExists(id), "No particle system exists with the given ID.");
//
//		return _systems.find(id)->second;
//
//	}
//	bool ParticleManager::SystemExists(ParticleSystemId id) const {
//
//		return _systems.count(id) > 0;
//
//	}
//	void ParticleManager::OnDraw(DrawEventArgs& e) {
//
//		_default_system.OnDraw(e);
//
//		for (auto i = _systems.begin(); i != _systems.end(); ++i)
//			i->second.OnDraw(e);
//
//	}
//	void ParticleManager::OnUpdate(UpdateEventArgs& e) {
//
//		_default_system.OnUpdate(e);
//
//		for (auto i = _systems.begin(); i != _systems.end(); ++i)
//			i->second.OnUpdate(e);
//
//	}
//
//}