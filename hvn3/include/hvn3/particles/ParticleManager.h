//#pragma once
//#include "hvn3/core/CoreDefs.h"
//#include "hvn3/core/IManager.h"
//#include "hvn3/core/ManagerDefs.h"
//#include "hvn3/particles/ParticleDefs.h"
//#include "hvn3/particles/ParticleSystem.h"
//#include "hvn3/particles/ParticleType.h"
//#include "hvn3/utility/Random.h"
//
//#include <unordered_map>
//
//namespace hvn3 {
//
//	class ParticleManager :
//		public IManager,
//		public IDrawable,
//		public IUpdatable {
//
//	public:
//		ParticleManager();
//
//		void RegisterType(ParticleTypeId id, const ParticleType& type);
//		ParticleType& GetType(ParticleTypeId id);
//		const ParticleType& GetType(ParticleTypeId id) const;
//		bool TypeIsRegistered(ParticleTypeId id) const;
//
//		ParticleSystem& DefaultSystem();
//		const ParticleSystem& DefaultSystem() const;
//
//		ParticleSystemHandle CreateSystem();
//		ParticleSystem& GetSystem(ParticleSystemId id);
//		const ParticleSystem& GetSystem(ParticleSystemId id) const;
//		bool SystemExists(ParticleSystemId id) const;
//
//		void OnDraw(DrawEventArgs& e) override;
//		void OnUpdate(UpdateEventArgs& e) override;
//
//	private:
//		ParticleSystem _default_system;
//		std::unordered_map<ParticleSystemId, ParticleSystem> _systems;
//		std::unordered_map<ParticleTypeId, ParticleType> _types;
//
//	};
//
//	template<>
//	struct ManagerIdTraits<PARTICLE_MANAGER> {
//		typedef ParticleManager type;
//	};
//
//}