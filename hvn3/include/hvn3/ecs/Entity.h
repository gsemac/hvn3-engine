#pragma once

namespace hvn3 {
	namespace ecs {

		struct Entity {

			typedef int entity_id;
			static const int NULL_ENTITY_ID;
			static Entity NULL_ENTITY;

			entity_id id;

			// #todo Add a timestamp member to detect if an entity with the same ID is newer than a previously-created entity.

			Entity() :
				Entity(NULL_ENTITY_ID) {}
			Entity(entity_id id) :
				id(id) {
			}

			explicit operator bool() const {
				return *this == NULL_ENTITY;
			}

		};

		const int Entity::NULL_ENTITY_ID = -1;
		Entity Entity::NULL_ENTITY = Entity(NULL_ENTITY_ID);

		bool operator==(const Entity& lhs, const Entity& rhs) {
			return lhs.id == rhs.id;
		}

	}
}