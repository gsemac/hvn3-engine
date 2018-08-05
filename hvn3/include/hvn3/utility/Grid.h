#pragma once
#include "hvn3/utility/Padding.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class Grid {

	public:
		Grid(unsigned int rows, unsigned int columns, float cell_width, float cell_height);
		unsigned int Rows() const;
		unsigned int Columns() const;
		float Width() const;
		float Height() const;
		float CellWidth() const;
		float CellHeight() const;
		SizeF CellSize() const;

	private:
		unsigned int _rows;
		unsigned int _columns;
		float _cell_width;
		float _cell_height;

	};

}