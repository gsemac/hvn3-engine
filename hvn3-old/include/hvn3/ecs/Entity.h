#pragma once

#include "hvn3/ecs/EcsDefs.h"

namespace hvn3 {
	namespace ecs {

		struct Entity {

			static const EntityId NULL_ENTITY_ID;
			static Entity NULL_ENTITY;

			EntityId id;

			// #todo Add a timestamp member to detect if an entity with the same ID is newer than a previously-created entity.

			Entity();
			Entity(EntityId id);

			explicit operator bool() const;

		};

		bool operator<(const Entity& lhs, const Entity& rhs);
		bool operator>(const Entity& lhs, const Entity& rhs);
		bool operator==(const Entity& lhs, const Entity& rhs);
		bool operator!=(const Entity& lhs, const Entity& rhs);

	}
}