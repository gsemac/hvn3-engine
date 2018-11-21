#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/ScrollableWidgetBase.h"

#include <cassert>
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

		public:
			DataGrid() {

				SetId("datagrid");

				_header_height = 20.0f;
				_row_height = 20.0f;

			}

			void AddColumn(const String& header_text) {

				DataGridColumn column;
				column.SetText(header_text);

				_columns.push_back(std::move(column));

			}
			DataGridColumn& Columns(int index) {

				assert(index > 0);
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

			void OnDraw(WidgetDrawEventArgs& e) override {

				e.Graphics().DrawSolidRectangle(X(), Y(), Width(), Height(), Color::White);

				float dx = X();

				for (auto i = _columns.begin(); i != _columns.end(); ++i) {

					e.Graphics().DrawSolidRectangle(dx, Y(), i->Width(), _header_height, Color::LtGrey);
					e.Graphics().DrawLine(dx + i->Width(), Y(), dx + i->Width(), Y() + Height(), Color::Silver, 1.0f);

					e.Graphics().DrawText(dx + (i->Width() / 2.0f), Y() + (_header_height / 2.0f), i->Text(), GetRenderer()->GetWidgetFont(this), Color::Black, Alignment::Center | Alignment::Middle);

					std::cout << i->Width();

					dx += i->Width();

				}

				for (float dy = Y() + _row_height; dy < Y() + Height(); dy += _row_height)
					e.Graphics().DrawLine(X(), dy, X() + Width(), dy, Color::Silver, 1.0f);

			}

		private:
			std::vector<DataGridColumn> _columns;
			float _header_height;
			float _row_height;

		};

	}
}