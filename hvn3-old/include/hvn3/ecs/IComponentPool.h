#pragma once

#include "hvn3/ecs/ComponentHandle.h"
#include "hvn3/ecs/Entity.h"
#include "hvn3/ecs/EntityComponentPair.h"
#include "hvn3/ecs/IComponent.h"

namespace hvn3 {
	namespace ecs {

		class ComponentManager;

		class IComponentPool {

			friend class ComponentManager;

		public:
			typedef Entity entity_type;

			// Returns true if there is a component in the pool corresponding to the given entity.
			virtual bool Exists(entity_type entity) const = 0;
			// Returns the number of components in the pool.
			virtual size_t Count() const = 0;

			// Returns the component from the pool corresponding to the given entity. Returns true if such a component existed; returns false otherwise.
			virtual bool Remove(entity_type entity) = 0;

			// Finds the component corresponding to the given entity, as an IComponent. Returns nullptr if such a cast is not possible.
			virtual IComponent* FindAsIComponent(entity_type entity) = 0;
			// Returns the entity-component pair at the given index, with the component as an IComponent. The component will be null if such a cast is not possible.
			virtual EntityComponentPair<IComponent*> AtAsIComponent(size_t index) = 0;

		};

	}
}