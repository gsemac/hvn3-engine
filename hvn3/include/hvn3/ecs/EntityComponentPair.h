#pragma once

#include "hvn3/ecs/EcsDefs.h"
#include "hvn3/ecs/Entity.h"

#include <cstdint>
#include <utility>

namespace hvn3 {
	namespace ecs {

		template<typename ComponentType, typename EntityType = Entity>
		class EntityComponentPair {

		public:
			typedef ComponentType component_type;
			typedef EntityType entity_type;
			typedef uint32_t timestamp_type;

			EntityComponentPair(entity_type entity, component_type&& component, timestamp_type timestamp) :
				entity(entity),
				component(std::move(component)),
				timestamp(timestamp) {}

			EntityType entity;
			component_type component;
			timestamp_type  timestamp;

		};

	}
}