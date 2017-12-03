#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/collision/SpacialPartitioningGrid.h"

namespace hvn3 {

	class SpacialPartitioningGridRenderer {

	public:
		SpacialPartitioningGridRenderer(Graphics::Graphics& graphics);

		void Draw(SpacialPartitioningGridRenderer& grid);

	private:
		Graphics::Graphics& _graphics;


	};

}