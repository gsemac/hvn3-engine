#pragma once

#include "hvn3/ecs/EcsDefs.h"

#include <cstdint>
#include <utility>

namespace hvn3 {
	namespace ecs {

		template<typename ComponentType, typename EntityType = EntityId>
		class EntityComponentPair {

		public:
			EntityComponentPair(EntityType entity, ComponentType&& component, uint32_t timestamp) :
				entity(entity),
				component(std::move(component)),
				timestamp(timestamp) {}

			EntityType entity;
			ComponentType component;
			uint32_t  timestamp;

		};

	}
}