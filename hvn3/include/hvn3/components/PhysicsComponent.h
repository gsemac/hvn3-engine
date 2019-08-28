#pragma once

#include "hvn3/components/ColliderComponent.h"
#include "hvn3/components/TransformComponent.h"
#include "hvn3/ecs/ComponentBase.h"

namespace hvn3 {

	class PhysicsComponent :
		public ecs::ComponentBase<TypeList<ColliderComponent, TransformComponent>> {

	public:


	private:

	};

}