#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <algorithm>
#include "GridVisualizer.h"
#include "Mouse.h"
#include "Graphics.h"

Gui::GridVisualizer::GridVisualizer() {}
Gui::GridVisualizer::GridVisualizer(Grid grid) {

	// Set private members.
	__grid = grid;
	__grid_bmp = nullptr;

	__scale = 1.0f;
	__outline_w = 1.0f;
	__grid_p = 1.0f;
	__draw_outline = false;

	__mouse_hover = true;

	__width = 0;
	__height = 0;

	__col_1 = al_map_rgb(216, 224, 248);
	__col_2 = al_map_rgb(232, 240, 248);
	__col_3 = al_map_rgb(248, 248, 248);
	__col_bg = al_map_rgb(168, 184, 216);
	__col_o = al_map_rgb(0, 0, 0);

}

void Gui::GridVisualizer::SetBackgroundColor(ALLEGRO_COLOR color) {

	__col_bg = color;

}
void Gui::GridVisualizer::SetOutlineColor(ALLEGRO_COLOR color) {

	__col_o = color;

}
void Gui::GridVisualizer::SetCellColor(ALLEGRO_COLOR color) {

	__col_1 = color;
	__col_2 = color;
	__col_3 = color;

}
void Gui::GridVisualizer::SetCellColor(ALLEGRO_COLOR primary_color, ALLEGRO_COLOR secondary_color, ALLEGRO_COLOR tertiary_color) {

	__col_1 = primary_color;
	__col_2 = secondary_color;
	__col_3 = tertiary_color;

}
void Gui::GridVisualizer::SetSchemeColor(int r, int g, int b) {

	__col_1 = al_map_rgb(r, g, b);
	__col_2 = al_map_rgb((std::min)(r + 16, 255), (std::min)(g + 16, 255), b);
	__col_3 = al_map_rgb(b, b, b);
	__col_bg = al_map_rgb((std::max)(r - 48, 0), (std::max)(g - 40, 0), (std::max)(b - 32, 0));
	__col_o = al_map_rgb((std::max)(r - 216, 0), (std::max)(g - 224, 0), (std::max)(b - 248, 0));

}

void Gui::GridVisualizer::SetOutlineWidth(float outline_width) {

	__outline_w = outline_width * __scale;

}
float Gui::GridVisualizer::OutlineWidth() {

	return __outline_w * __scale;

}
void Gui::GridVisualizer::SetScale(float scale) {

	__scale = scale;
	Redraw();

}

void Gui::GridVisualizer::GenerateBitmap() {

	// Initialize positioning values.
	float grid_x = __grid_p * __scale;
	float grid_y = __grid_p * __scale;
	float grid_space_w = __grid.Cells().Width() * __scale;
	float grid_space_h = grid_space_w;
	float grid_space_m = __grid.Cells().Margin() * __scale;
	float grid_outline_w = __outline_w * __scale;
	bool col_switch = 0;

	// Create the bitmap, disposing of the old one if needed.
	if (__grid_bmp) al_destroy_bitmap(__grid_bmp);
	__width = __grid.Width() * __scale + grid_outline_w * 2.0f;
	__height = __grid.Height() * __scale + grid_outline_w * 2.0f;
	__grid_bmp = al_create_bitmap(__width, __height);

	// Set the bitmap as the drawing target.
	al_set_target_bitmap(__grid_bmp);

	// Draw the board background.
	al_clear_to_color(__col_bg);

	// Draw the square spaces.
	for (int i = 0; i < __grid.Columns(); ++i) {
		for (int j = 0; j < __grid.Rows(); ++j) {

			float x1 = grid_x + i * grid_space_w + i * grid_space_m;
			float y1 = grid_y + j * grid_space_w + j * grid_space_m;
			float x2 = x1 + grid_space_w;
			float y2 = y1 + grid_space_h;

			al_draw_filled_rectangle(x1, y1, x2, y2,
				(col_switch) ? ((j % 2 == 0) ? __col_2 : __col_3) : ((j % 2 == 0) ? __col_1 : __col_2));

		}
		col_switch = !col_switch;

	}

	// Draw the outline.
	if (__draw_outline && grid_outline_w > 0) {
		al_draw_rectangle(
			grid_outline_w / 2.0f,
			grid_outline_w / 2.0f,
			__grid.Width() * __scale + grid_outline_w * 1.5f,
			__grid.Height() * __scale + grid_outline_w * 1.5f,
			__col_o, grid_outline_w);
	}

	Graphics::ResetDrawingTarget();

}
void Gui::GridVisualizer::Draw() {

	// Draw the grid.
	if (!__grid_bmp) GenerateBitmap();
	al_draw_bitmap(__grid_bmp, X, Y, NULL);

}
Point Gui::GridVisualizer::MouseOnCell() {

	// If there isn't a visible grid, don't bother.
	if (!__grid_bmp) return Point(-1.0f, -1.0f);

	// Caculate the mouse's relative position to the grid.
	float mouse_x_rel = Mouse::X - (X + (__grid.Cells().Margin() * __scale) / 2.0f);
	float mouse_y_rel = Mouse::Y - (Y + (__grid.Cells().Margin() * __scale) / 2.0f);
	if (
		mouse_x_rel < __grid_p || mouse_y_rel < __grid_p ||
		mouse_x_rel > __width - __outline_w * __scale - 1.0f ||
		mouse_y_rel > __height - __outline_w * __scale - 1.0f
		) return Point(-1.0f, -1.0f);

	// Return the row/col position of the cell.
	int cell_x = mouse_x_rel / (__grid.Cells().Width() * __scale + (__grid.Cells().Margin() * __scale));
	int cell_y = mouse_y_rel / (__grid.Cells().Height() * __scale + (__grid.Cells().Margin() * __scale));

	// Return the result.
	return Point(cell_x, cell_y);

}
bool Gui::GridVisualizer::IsValidCell(Point& point) {

	return !(point.X < 0 && point.Y < 0);

}
void Gui::GridVisualizer::Redraw() {

	// Destroy the current bitmap so that it will be created when next drawn.
	if (__grid_bmp) al_destroy_bitmap(__grid_bmp);
	__grid_bmp = nullptr;

}

float Gui::GridVisualizer::RowOffset(int row, Alignment align) {

	float offset = (row * __grid.Cells().Height() + row * __grid.Cells().Margin() + __grid_p) * __scale;
	switch (align) {
	case Alignment::Center: return offset + (__grid.Cells().Height() / 2.0f) * __scale;
	case Alignment::Bottom: return offset + (__grid.Cells().Height()) * __scale;
	default: return offset;
	}

}
float Gui::GridVisualizer::ColumnOffset(int column, Alignment align) {

	float offset = (column * __grid.Cells().Width() + column * __grid.Cells().Margin() + __grid_p) * __scale;
	switch (align) {
	case Alignment::Center: return offset + (__grid.Cells().Width() / 2.0f) * __scale;
	case Alignment::Right: return offset + (__grid.Cells().Width()) * __scale;
	default: return offset;
	}

}
float Gui::GridVisualizer::ColumnWidth() {

	return __grid.Cells().Width() * __scale;

}
float Gui::GridVisualizer::RowHeight() {

	return __grid.Cells().Height() * __scale;

}
Point Gui::GridVisualizer::CellOffset(Point& position) {

	Point p;
	p.X = (__grid_p + position.X * (__grid.Cells().Width() + __grid.Cells().Margin())) * __scale; // +(__outline_w * __scale); // / 2.0f;
	p.Y = (__grid_p + position.Y * (__grid.Cells().Height() + __grid.Cells().Margin())) * __scale; // +(__outline_w * __scale); // / 2.0f;
	return p;

}
Rectangle Gui::GridVisualizer::CellBounds(Point& position) {

	Point cell_offset = CellOffset(position);
	Rectangle r(
		cell_offset.X,
		cell_offset.Y,
		__grid.Cells().Width() * __scale - (__outline_w * __scale), 
		__grid.Cells().Height() * __scale - (__outline_w * __scale));
	return r;

}
float Gui::GridVisualizer::Width() {

	return __grid.Width() * __scale + (__outline_w * __scale) * 2.0f;

}
float Gui::GridVisualizer::Height() {

	return __grid.Height() * __scale + (__outline_w * __scale) * 2.0f;

} 

Gui::GridVisualizer::~GridVisualizer() {

	// Delete the existing bitmap if needed.
	if (__grid_bmp) al_destroy_bitmap(__grid_bmp);

}