#pragma once

#include "hvn3/ecs/IComponent.h"
#include "hvn3/sprites/SpriteRenderer.h"

namespace hvn3 {

	class SpriteComponent :
		public ecs::IComponent,
		public SpriteRenderer {
	};

}