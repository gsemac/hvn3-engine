#pragma once
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/particles/ParticleSystem.h"

namespace hvn3 {

	class ParticleManager :
		public IManager,
		public IDrawable,
		public IUpdatable {

	public:
		const ParticleSystem& DefaultSystem() const {
			return _default_system;
		}
		ParticleSystem& DefaultSystem() {
			return _default_system;
		}

		void OnDraw(DrawEventArgs& e) override {

			_default_system.OnDraw(e);

			for (auto i = _systems.begin(); i != _systems.end(); ++i)
				i->OnDraw(e);

		}
		void OnUpdate(UpdateEventArgs& e) override {

			_default_system.OnUpdate(e);

			for (auto i = _systems.begin(); i != _systems.end(); ++i)
				i->OnUpdate(e);

		}

	private:
		ParticleSystem _default_system;
		std::vector<ParticleSystem> _systems;

	};

	template<>
	struct ManagerIdTraits<PARTICLE_MANAGER> {
		typedef ParticleManager type;
	};

}