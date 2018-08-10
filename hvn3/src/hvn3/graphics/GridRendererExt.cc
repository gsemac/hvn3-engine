#include "hvn3/graphics/GridRendererExt.h"
#include <cmath>

namespace hvn3 {
	namespace Graphics {

		GridRendererExt::GridRendererExt() :
			_pen(Color::Black),
			_last_cell_size(0.0f, 0.0f) {

			SetCellColor(Color::Transparent);

			_cell_margin = 0.0f;
			_buffer_invalidated = true;

		}
		void GridRendererExt::Draw(Graphics& canvas, const PointF& position, const Grid& grid) {

			if (_buffer_invalidated || _last_cell_size != grid.CellSize()) {

				_refreshBuffer(grid);

				_last_cell_size = grid.CellSize();

			}

			if (!_buffer)
				return;
			
			size_t buffer_index = 0;
		
			float x = position.x;
			float y = position.y;
			SizeF cell_size = _getActualCellSize(grid);
			float cell_w = cell_size.width;
			float cell_h = cell_size.height;
			float w = grid.Columns() * cell_w - _cell_margin;
			float h = grid.Rows() * cell_h - _cell_margin;
			
			bool skip = (grid.Columns() % _cell_colors.size()) == 0;

			GraphicsState state = canvas.Save();
			
			canvas.HoldBitmapDrawing(true);
			//canvas.SetClip(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));

			for (unsigned int cy = 0; cy < grid.Rows(); ++cy) {
				for (unsigned int cx = 0; cx < grid.Columns(); ++cx) {

					if (buffer_index >= _cell_colors.size())
						buffer_index %= _cell_colors.size();

					canvas.DrawBitmap(
						x + (static_cast<float>(cx) * cell_w),
						y + (static_cast<float>(cy) * cell_h),
						_buffer,
						RectangleF(buffer_index * cell_w, 0.0f, cell_w, cell_h));

					++buffer_index;

				}

				if (skip)
					++buffer_index;

			}

			canvas.HoldBitmapDrawing(false);
			canvas.Restore(state);

		}
		void GridRendererExt::SetLineStyle(const Pen& value) {

			_pen = value;

			_buffer_invalidated = true;

		}
		void GridRendererExt::SetCellColor(const Color& value) {

			_cell_colors.clear();
			_cell_colors.push_back(value);

			_buffer_invalidated = true;

		}
		void GridRendererExt::SetCellColors(std::initializer_list<Color> value) {

			_cell_colors.clear();

			for (auto i = value.begin(); i != value.end(); ++i)
				_cell_colors.push_back(*i);

			if (_cell_colors.size() <= 0)
				SetCellColor(Color::Transparent);

			_buffer_invalidated = true;

		}
		void GridRendererExt::SetCellMargin(float value) {

			_cell_margin = value;

			_buffer_invalidated = true;

		}

		void GridRendererExt::_refreshBuffer(const Grid& grid) {

			// The buffer contains each possible cell.

			SizeF actual_cs = _getActualCellSize(grid);
			float cell_w = grid.CellWidth();
			float cell_h = grid.CellHeight();
			int buf_w = static_cast<int>(actual_cs.width * _cell_colors.size());
			int buf_h = static_cast<int>(actual_cs.height);

			if (buf_w <= 0 || buf_h <= 0)
				return;

			if (_buffer.Width() != buf_w || _buffer.Height() != buf_h)
				_buffer = Bitmap(buf_w, buf_h);

			Graphics g(_buffer);
			g.Clear(Color::Transparent);

			for (size_t i = 0; i < static_cast<size_t>(_buffer.Width()) / static_cast<size_t>(cell_w); ++i) {

				float dx = i * actual_cs.width;
				float dy = 0.0f;

				g.DrawSolidRectangle(dx, dy, cell_w, cell_h, _cell_colors[i]);

				g.DrawLine(dx, dy + actual_cs.height - (_cell_margin / 2.0f), dx + actual_cs.width, dy + actual_cs.height - (_cell_margin / 2.0f), _pen); // bottom
				g.DrawLine(dx + actual_cs.width - (_cell_margin / 2.0f), dy, dx + actual_cs.width - (_cell_margin / 2.0f), dy + actual_cs.height, _pen); // right

			}

		}
		SizeF GridRendererExt::_getActualCellSize(const Grid& grid) {

			float cell_w = grid.CellWidth() + _cell_margin;
			float cell_h = grid.CellHeight() + _cell_margin;

			return SizeF(cell_w, cell_h);

		}

	}
}