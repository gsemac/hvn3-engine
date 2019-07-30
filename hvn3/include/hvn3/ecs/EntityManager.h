#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/ecs/IEntityManager.h"
#include "hvn3/utility/UniqueIntegerGenerator.h"

namespace hvn3 {
	namespace ecs {

		class EntityManager :
			public ManagerBase<IEntityManager> {

		public:
			Entity CreateEntity() override {

				Entity entity(_entity_generator.Next());

				return entity;

			}
			void FreeEntity(const Entity& entity) override {
				_entity_generator.Free(entity.id);
			}

		private:
			UniqueIntegerGenerator<Entity::entity_id> _entity_generator;

		};

	}
}