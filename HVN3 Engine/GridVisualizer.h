#ifndef __GRID_VISUALIZER
#define __GRID_VISUALIZER
#include <allegro5/allegro.h>
#include "IDrawable.h"
#include "Grid.h"
#include "Rectangle.h"
#include "Helper.h"

class Grid;

namespace Gui {

	class GridVisualizer : public IDrawable, public IPositionable {

	private:
		Grid __grid;
		ALLEGRO_BITMAP* __grid_bmp;

		bool __draw_outline;
		bool __mouse_hover;
		float __scale, __outline_w, __grid_p;
		float __width, __height;
		ALLEGRO_COLOR __col_1, __col_2, __col_3, __col_bg, __col_o;
	
		void GenerateBitmap();

	public:
		GridVisualizer();
		GridVisualizer(Grid grid);
		void Draw();
		void Redraw();
		~GridVisualizer();

		void SetOutlineWidth(float outline_width);
		float OutlineWidth();
		void SetBackgroundColor(ALLEGRO_COLOR color);
		void SetOutlineColor(ALLEGRO_COLOR color);
		void SetCellColor(ALLEGRO_COLOR primary_color);
		void SetCellColor(ALLEGRO_COLOR primary_color, ALLEGRO_COLOR secondary_color, ALLEGRO_COLOR tertiary_color);
		void SetSchemeColor(int r, int g, int b);
		void SetScale(float scale);

		Point MouseOnCell();
		bool IsValidCell(Point& point);
		float RowOffset(int row, Alignment align = Alignment::Top);
		float ColumnOffset(int column, Alignment align = Alignment::Left);
		float ColumnWidth();
		float RowHeight();
		Point CellOffset(Point& position);
		Rectangle CellBounds(Point& position);
		float Width();
		float Height();

	};

}

#endif