#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/utility/Grid.h"
#include "hvn3/graphics/Color.h"

namespace hvn3 {
	namespace Graphics {

		class GridRenderer {

		public:
			GridRenderer(Graphics& graphics);

			// Draws a simple transparent grid with black lines.
			void Draw(const Point2d<float>& position, const Grid& grid);
			// Draws a simple transparent grid with lines of the specified color.
			void Draw(const Point2d<float>& position, const Grid& grid, const Color& grid_color);
			// Draws a simple grid with the specified line and cell colors.
			void Draw(const Point2d<float>& position, const Grid& grid, const Color& grid_color, const Color& cell_color);

		private:
			Graphics& _graphics;

		};

	}
}