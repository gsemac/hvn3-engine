#ifndef __GRID_H
#define __GRID_H
#include <vector>
#include "IPositionable.h"

namespace hvn3 {

	class Grid {

	public:
		class CellProperties {
			friend class Grid;
		private:
			float __cell_w, __cell_h, __cell_m, __cell_p;
		public:
			float Width();
			float Height();
			float Margin();
			float Padding();
		};

	protected:
		int __rows, __columns;
		CellProperties __cells;

	public:
		Grid();
		Grid(int rows, int columns);
		Grid(int rows, int columns, float cell_width, float cell_height, float cell_margin, float cell_padding);
		int Rows();
		int Columns();
		float Width();
		float Height();
		virtual void Resize(int rows, int columns);
		void ResizeCells(float cell_width, float cell_height, float cell_margin, float cell_padding);
		CellProperties& Cells();

	};

}

#endif