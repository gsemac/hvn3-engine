#include "utility/Grid.h"

namespace hvn3 {

	Grid::Grid(unsigned int rows, unsigned int columns, float cell_width, float cell_height) {

		_rows = rows;
		_columns = columns;

		_cell_width = cell_width;
		_cell_height = cell_height;

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

}