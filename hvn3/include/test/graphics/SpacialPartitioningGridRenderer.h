#pragma once
#include "graphics/Graphics.h"
#include "collision/SpacialPartitioningGrid.h"

namespace hvn3 {

	class SpacialPartitioningGridRenderer {

	public:
		SpacialPartitioningGridRenderer(Graphics::Graphics& graphics);

		void Draw(SpacialPartitioningGridRenderer& grid);

	private:
		Graphics::Graphics& _graphics;


	};

}