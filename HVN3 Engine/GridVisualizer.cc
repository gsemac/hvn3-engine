#include "GridVisualizer.h"

namespace hvn3 {
	namespace Drawing {

		// Public methods

		GridVisualizer::GridVisualizer(Graphics& graphics) :
			_graphics(graphics) {
		}

		void GridVisualizer::DrawGrid(const Point& position, const Grid& grid) {

			DrawGrid(position, grid, Color::Black);

		}
		void GridVisualizer::DrawGrid(const Point& position, const Grid& grid, const Color& grid_color) {

			for (unsigned int i = 1; i < grid.Columns(); ++i)
				_graphics.DrawLine(position.X() + grid.CellWidth() * i, position.Y(), position.X() + grid.CellWidth() * i, position.Y() + grid.Height(), grid_color, 1.0f);

			for (unsigned int i = 1; i < grid.Rows(); ++i)
				_graphics.DrawLine(position.X(), position.Y() + grid.CellHeight() * i, position.X() + grid.Width(), position.Y() + grid.CellHeight() * i, grid_color, 1.0f);

		}
		void GridVisualizer::DrawGrid(const Point& position, const Grid& grid, const Color& grid_color, const Color& cell_color) {

			_graphics.DrawFilledRectangle(position.X(), position.Y(), grid.Width(), grid.Height(), cell_color);

			DrawGrid(position, grid, grid_color);

		}

		//void GridVisualizer::DrawGrid(Graphics& graphics, const Point& position, const Grid& grid) {

		//	if (!Color::IsTranluscent(_line_color)) {

		//		// If the line color is opaque, we can simplify drawing dramatically.

		//		// Start by filling the drawing area with the cell color.
		//		graphics.DrawFilledRectangle(position.X(), position.Y(), grid.Width(), grid.Height(), _cell_color);

		//		//// Next, draw the grid lines over top of the drawing area.
		//		//float x = position.X();
		//		//float y = position.Y();
		//		//float w = grid.CellMargin().Left();
		//		//float h = grid.Height();

		//		//do {

		//		//	// Draw the next "line" of the grid.
		//		//	graphics.DrawFilledRectangle(x, y, w, h, _line_color);

		//		//	// If we've reached the end of the grid, stop drawing.
		//		//	if (x + w == position.X() + grid.Width())
		//		//		break;

		//		//	x += w + grid.CellSize().Width();
		//		//	w = grid.CellMargin().Left() + grid.CellMargin().Right();

		//		//	if (x + w > position.X() + grid.Width())
		//		//		w = (position.X() + grid.Width()) - x;

		//		//} while (1);

		//		//x = position.X();
		//		//y = position.Y();
		//		//w = grid.Width();
		//		//h = grid.CellMargin().Top();

		//		//do {

		//		//	// Draw the next "line" of the grid.
		//		//	graphics.DrawFilledRectangle(x, y, w, h, _line_color);

		//		//	// If we've reached the end of the grid, stop drawing.
		//		//	if (y + h == position.Y() + grid.Height())
		//		//		break;

		//		//	y += h + grid.CellSize().Height();
		//		//	h = grid.CellMargin().Top() + grid.CellMargin().Bottom();

		//		//	if (y + h > position.Y() + grid.Height())
		//		//		h = (position.Y() + grid.Height()) - y;

		//		//} while (1);




		//		//for(float x = grid.CellMargin().Left(); x < grid.Width(); x += grid.CellSize().Width() + grid.CellMargin().Left() + grid.CellMargin().Right())


		//	}
		//	else {

		//		// Otherwise, we need to draw each cell separately.


		//	}


		//	// Draw the background color of the grid (the cell color).


		//	// Draw the lines between cells.


		//}

	}
}