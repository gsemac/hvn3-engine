#include "Grid.h"

Grid::Grid(int rows, int columns, float cell_width, float cell_height, float cell_margin, float cell_padding) {

	// Apply provided values.
	__rows = rows;
	__columns = columns;
	__cells.__cell_w = cell_width;
	__cells.__cell_h = cell_height;
	__cells.__cell_m = cell_margin;
	__cells.__cell_p = cell_padding;

}
Grid::Grid(int rows, int columns) : Grid::Grid(rows, columns, 15.0f, 15.0f, 1.0f, 0) {}
Grid::Grid() : Grid::Grid(0, 0) {}

int Grid::Rows() {

	return __rows;

}
int Grid::Columns() {

	return __columns;

}
void Grid::Resize(int rows, int columns) {

	__rows = rows;
	__columns = columns;

}
void Grid::ResizeCells(float cell_width, float cell_height, float cell_margin, float cell_padding) {

	__cells.__cell_w = cell_width;
	__cells.__cell_h = cell_height;
	__cells.__cell_m = cell_margin;
	__cells.__cell_p = cell_padding;

}
float Grid::Width() {

	return (__columns * __cells.Width()) + ((__columns - 1) * __cells.Margin());

}
float Grid::Height() {

	return (__rows * __cells.Height()) + ((__rows - 1) * __cells.Margin());

}

Grid::CellProperties& Grid::Cells() {

	return __cells;

}
float Grid::CellProperties::Width() {

	return __cell_w;

}
float Grid::CellProperties::Height() {

	return __cell_w;

}
float Grid::CellProperties::Margin() {

	return __cell_m;

}
float Grid::CellProperties::Padding() {

	return __cell_p;

}