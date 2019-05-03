#include "hvn3/ecs/Entity.h"

namespace hvn3 {
	namespace ecs {

		// Public constants

		const int Entity::NULL_ENTITY_ID = -1;
		Entity Entity::NULL_ENTITY = Entity(NULL_ENTITY_ID);

		// Public methods

		Entity::Entity() :
			Entity(NULL_ENTITY_ID) {}
		Entity::Entity(entity_id id) :
			id(id) {
		}

		Entity::operator bool() const {
			return *this == NULL_ENTITY;
		}

		// Free functions

		bool operator==(const Entity& lhs, const Entity& rhs) {
			return lhs.id == rhs.id;
		}
		bool operator!=(const Entity& lhs, const Entity& rhs) {
			return !(lhs == rhs);
		}

	}
}