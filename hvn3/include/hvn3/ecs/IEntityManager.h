#pragma once

#include "hvn3/ecs/Entity.h"

namespace hvn3 {
	namespace ecs {

		class IEntityManager {

		public:
			virtual Entity CreateEntity() = 0;
			virtual void FreeEntity(const Entity& entity) = 0;

		};

	}
}