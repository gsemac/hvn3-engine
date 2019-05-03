#pragma once

namespace hvn3 {
	namespace ecs {

		struct Entity {

			typedef int entity_id;
			static const int NULL_ENTITY_ID;
			static Entity NULL_ENTITY;

			entity_id id;

			// #todo Add a timestamp member to detect if an entity with the same ID is newer than a previously-created entity.

			Entity();
			Entity(entity_id id);

			explicit operator bool() const;

		};

		bool operator==(const Entity& lhs, const Entity& rhs);
		bool operator!=(const Entity& lhs, const Entity& rhs);

	}
}