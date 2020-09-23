#include "hvn3/utility/Grid.h"
#include <algorithm>

namespace hvn3 {

	Grid::Grid(unsigned int rows, unsigned int columns, float cell_width, float cell_height) {

		_rows = rows;
		_columns = columns;
		_cell_width = cell_width;
		_cell_height = cell_height;

	}
	Grid::Grid(SizeF grid_size, SizeF cell_size, bool roundUpRowsAndColumns) {

		float rows = grid_size.height / cell_size.height;
		float cols = grid_size.width / cell_size.width;

		if (roundUpRowsAndColumns) {
			rows = (std::ceil)(rows);
			cols = (std::ceil)(cols);
		}

		_rows = static_cast<unsigned int>(rows);
		_columns = static_cast<unsigned int>(cols);
		_cell_width = cell_size.width;
		_cell_height = cell_size.height;

	}

	unsigned int Grid::Rows() const {

		return _rows;

	}
	unsigned int Grid::Columns() const {

		return _columns;

	}

	float Grid::Width() const {

		return (_columns * _cell_width);
	}
	float Grid::Height() const {

		return (_rows * _cell_height);

	}
	float Grid::CellWidth() const {

		return _cell_width;

	}
	float Grid::CellHeight() const {

		return _cell_height;

	}
	SizeF Grid::CellSize() const {
		return SizeF(_cell_width, _cell_height);
	}

}