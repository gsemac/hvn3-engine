#pragma once
#include "graphics/Graphics.h"
#include "collision/SpacialPartitioningGrid.h"

namespace hvn3 {

	class SpacialPartitioningGridRenderer {

	public:
		SpacialPartitioningGridRenderer(Drawing::Graphics& graphics);

		void Draw(SpacialPartitioningGridRenderer& grid);

	private:
		Drawing::Graphics& _graphics;


	};

}