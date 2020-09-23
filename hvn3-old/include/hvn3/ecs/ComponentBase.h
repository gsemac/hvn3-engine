#pragma once

#include "hvn3/ecs/IComponent.h"
#include "hvn3/utility/TypeList.h"

namespace hvn3 {
	namespace ecs {

		template<typename RequiredComponentTypes = TypeList<>>
		class ComponentBase :
			public IComponent {

		public:
			typedef RequiredComponentTypes required_component_types;

		};

	}
}