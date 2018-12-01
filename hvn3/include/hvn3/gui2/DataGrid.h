#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/ScrollableWidgetBase.h"
#include "hvn3/gui2/TextBox.h"

#include <cassert>
#include <initializer_list>
#include <vector>

namespace hvn3 {
	namespace Gui {

		class DataGridColumn {

		public:
			DataGridColumn() {

				_width = 100.0f;

			}

			const String& Text() const {
				return _text;
			}
			void SetText(const String& value) {
				_text = value;
			}
			float Width() const {
				return _width;
			}
			void SetWidth(float value) {
				_width = value;
			}

		private:
			String _text;
			float _width;

		};

		class DataGrid :
			public ScrollableWidgetBase {

			typedef std::vector<String> row_type;

		public:
			DataGrid() {

				SetId("datagrid");

				_header_height = 25.0f;
				_row_height = 25.0f;

				_current_edit_box = nullptr;
				_current_edit_index = PointI(-1, -1);

			}

			void AddColumn(const String& header_text) {

				DataGridColumn column;
				column.SetText(header_text);

				_columns.push_back(std::move(column));

			}
			DataGridColumn& Columns(int index) {

				assert(index >= 0);
				assert(static_cast<size_t>(index) < _columns.size());

				return _columns[static_cast<size_t>(index)];

			}
			DataGridColumn& Columns(const String& header_text) {

				auto it = std::find_if(_columns.begin(), _columns.end(), [&](const DataGridColumn& column) {
					return (column.Text() == header_text);
				});

				assert(it != _columns.end());

				return *it;

			}
			void AddRow() {

				_rows.push_back(row_type());

			}
			void AddRow(std::initializer_list<String> values) {

				AddRow();

				for (auto i = values.begin(); i != values.end(); ++i)
					_rows.back().push_back(*i);

			}

			size_t ColumnCount() const {

				return _columns.size();

			}
			size_t RowCount() const {

				return _rows.size();

			}

			void OnDraw(WidgetDrawEventArgs& e) override {

				Color row_color = Color::White;
				Color background_color(171, 171, 171);
				Color header_color(252, 252, 252);
				Color grid_line_color(235, 244, 254);

				float dx = X();
				float dy = Y();

				// Draw background.
				e.Graphics().DrawSolidRectangle(dx, dy, Width(), Height(), background_color);

				// Draw column headers.

				for (auto i = _columns.begin(); i != _columns.end(); ++i) {

					e.Graphics().DrawSolidRectangle(dx, dy, i->Width(), _header_height, header_color); // background
					e.Graphics().DrawLine(dx + i->Width(), dy, dx + i->Width(), dy + _header_height, grid_line_color, 1.0f); // vertical divider
					e.Graphics().DrawText(dx + (i->Width() / 2.0f), dy + (_header_height / 2.0f), i->Text(), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Center | Alignment::Middle);

					dx += i->Width();

				}

				e.Graphics().DrawLine(X(), Y() + _header_height, dx, Y() + _header_height, grid_line_color, 1.0f); // horizontal divider

				// Draw rows.

				dy = Y() + _header_height;

				for (auto i = _rows.begin(); i != _rows.end(); ++i) {

					dx = X();
					size_t column_index = 0;

					for (auto j = _columns.begin(); j != _columns.end(); ++j) {

						e.Graphics().DrawSolidRectangle(dx, dy, j->Width(), _row_height, row_color); // background
						e.Graphics().DrawLine(dx + j->Width(), dy, dx + j->Width(), dy + _row_height, grid_line_color, 1.0f); // vertical divider

						if (column_index < i->size())
							e.Graphics().DrawText(dx + (j->Width() / 2.0f), dy + (_row_height / 2.0f), i->at(column_index), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Center | Alignment::Middle);

						dx += j->Width();
						++column_index;

					}

					e.Graphics().DrawLine(X(), dy + _row_height, dx, dy + _row_height, grid_line_color, 1.0f); // horizontal divider

					dy += _row_height;

				}

			}
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override {

				// If the mouse is on one of the header dividers, switch to resize cursor.

				SetCursor(SystemCursor::Default);

				PointF p = FixedPosition();
				float resize_region_padding = 2.0f;

				for (auto i = _columns.begin(); i != _columns.end(); ++i) {

					RectangleF resize_region(p.x + i->Width() - resize_region_padding, p.y, resize_region_padding * 2.0f, _header_height);

					if (e.Position().In(resize_region)) {

						SetCursor(SystemCursor::ResizeE);

						break;

					}

					p.x += i->Width();

				}

			}
			void OnMousePressed(WidgetMousePressedEventArgs& e) override {

				// If a cell was clicked, begin editing that cell.

				_endEdit();

				PointI cell_index = _globalPositionToCellIndex(e.Position());

				if (cell_index.x < 0 || cell_index.y < 0)
					return;

				PointF tb_pos = Position();
				TextBox* tb = new TextBox(_columns[cell_index.x].Width());

				for (int i = 0; i < cell_index.x; ++i)
					tb_pos.x += Columns(i).Width();

				tb_pos.y += _header_height;
				tb_pos.y += _row_height * static_cast<float>(cell_index.y);

				tb->SetPosition(tb_pos);
				tb->SetSize(_columns[cell_index.x].Width() - 1.0f, _row_height - 1.0f);
				tb->SetText(_getDataAt(cell_index));

				GetManager()->Add(tb);

				_current_edit_box = tb;
				_current_edit_index = cell_index;

			}
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override {

				if (_current_edit_box != nullptr)
					_current_edit_box->Focus();

			}

		private:
			std::vector<DataGridColumn> _columns;
			std::vector<row_type> _rows;
			float _header_height;
			float _row_height;

			TextBox* _current_edit_box;
			PointI _current_edit_index;

			PointI _globalPositionToCellIndex(const PointF& point) {

				PointI result(-1, -1);

				if (!point.In(Bounds()))
					return result;

				PointF comparison_pos = FixedPosition();

				// Find the column that the point is in.

				int column_index = 0;

				for (auto i = _columns.begin(); i != _columns.end(); ++i) {

					if (point.x >= comparison_pos.x && point.x < comparison_pos.x + i->Width()) {

						result.x = column_index;
						break;

					}

					comparison_pos.x += i->Width();
					++column_index;

				}

				// Find the row that the point is in.

				comparison_pos.y += _header_height;

				int row_index = 0;

				for (auto i = _rows.begin(); i != _rows.end(); ++i) {

					if (point.y >= comparison_pos.y && point.y < comparison_pos.y + _row_height) {

						result.y = row_index;
						break;

					}

					comparison_pos.y += _row_height;
					++row_index;

				}

				return result;

			}
			void _endEdit() {

				if (_current_edit_box == nullptr)
					return;

				assert(_current_edit_box != nullptr);
				assert(_current_edit_index.x >= 0);
				assert(_current_edit_index.y >= 0);
				assert(static_cast<size_t>(_current_edit_index.y) < _rows.size());

				auto& row = _rows[_current_edit_index.y];

				while (row.size() <= static_cast<size_t>(_current_edit_index.x))
					row.push_back("");

				row[_current_edit_index.x] = _current_edit_box->Text();

				GetManager()->Remove(_current_edit_box);

				_current_edit_box = nullptr;
				_current_edit_index = PointI(-1, -1);

			}
			String _getDataAt(const PointI& cell_index) const {

				if (cell_index.x < 0 ||
					cell_index.y < 0 ||
					static_cast<size_t>(cell_index.x) >= _columns.size() ||
					static_cast<size_t>(cell_index.y) >= _rows.size())
					return "";

				auto& row = _rows[cell_index.y];

				if (static_cast<size_t>(cell_index.x) >= row.size())
					return "";

				return row[cell_index.x];

			}

		};

	}
}