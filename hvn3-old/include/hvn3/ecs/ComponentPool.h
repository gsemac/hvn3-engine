#pragma once

#include "hvn3/ecs/ComponentPoolBase.h"
#include "hvn3/ecs/IComponent.h"

#include <type_traits>

namespace hvn3 {
	namespace ecs {

		template <typename T, typename Enable = void>
		class ComponentPool;

		template <typename ComponentType>
		class ComponentPool<ComponentType, typename std::enable_if<std::is_base_of<IComponent, ComponentType>::value>::type> :
			public ComponentPoolBase<ComponentType> {

		protected:
			IComponent* FindAsIComponent(entity_type entity) override {

				return static_cast<IComponent*>(Find(entity));

			}
			EntityComponentPair<IComponent*> AtAsIComponent(size_t index) override {

				auto it = GetComponents().begin() + index;

				return EntityComponentPair<IComponent*>(it->entity, static_cast<IComponent*>(&it->component), it->timestamp);

			}

		};

		template <typename ComponentType>
		class ComponentPool<ComponentType, typename std::enable_if<!std::is_base_of<IComponent, ComponentType>::value>::type> :
			public ComponentPoolBase<ComponentType> {
		};

	}
}