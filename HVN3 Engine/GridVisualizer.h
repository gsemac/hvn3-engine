#pragma once
#include "Graphics.h"
#include "Grid.h"
#include "Color.h"

namespace hvn3 {
	namespace Drawing {

		class GridVisualizer {

		public:
			GridVisualizer(Graphics& graphics);

			// Draws a simple transparent grid with black lines.
			void DrawGrid(const Point& position, const Grid& grid);
			// Draws a simple transparent grid with lines of the specified color.
			void DrawGrid(const Point& position, const Grid& grid, const Color& grid_color);
			// Draws a simple grid with the specified line and cell colors.
			void DrawGrid(const Point& position, const Grid& grid, const Color& grid_color, const Color& cell_color);

		private:
			Graphics& _graphics;

		};

	}
}