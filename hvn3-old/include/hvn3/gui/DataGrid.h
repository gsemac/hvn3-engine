#pragma once

#include "hvn3/gui/ScrollableWidgetBase.h"
#include "hvn3/gui/TextBox.h"

#include <initializer_list>
#include <vector>

namespace hvn3 {
	namespace Gui {

		class DataGridColumn {

		public:
			DataGridColumn();

			const String& Text() const;
			void SetText(const String& value);
			float Width() const;
			void SetWidth(float value);

		private:
			String _text;
			float _width;

		};

		class DataGrid;

		class DataGridRow {

		public:
			DataGridRow(DataGrid* parent, int row_index);

			const String& Values(int index) const;
			size_t Count() const;

			const String& operator[](int index) const;

		private:
			DataGrid* _parent;
			int _row_index;

		};

		class DataGrid :
			public ScrollableWidgetBase {

			friend class DataGridRow;

			typedef std::vector<String> row_type;

		public:
			DataGrid();

			void AddColumn(const String& header_text);
			DataGridColumn& Columns(int index);
			DataGridColumn& Columns(const String& header_text);
			void AddRow();
			void AddRow(std::initializer_list<String> values);
			DataGridRow Rows(int index);

			size_t ColumnCount() const;
			size_t RowCount() const;

			void OnDraw(WidgetDrawEventArgs& e) override;
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override;
			void OnMousePressed(WidgetMousePressedEventArgs& e) override;
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;

		private:
			std::vector<DataGridColumn> _columns;
			std::vector<row_type> _rows;
			float _header_height;
			float _row_height;

			TextBox* _current_edit_box;
			PointI _current_edit_index;
			bool _allow_end_edit;

			PointI _globalPositionToCellIndex(const PointF& point);
			void _beginEdit(const PointI& cell_index);
			void _commitEdit();
			void _endEdit();
			String _getDataAt(const PointI& cell_index) const;
			void _updateScrollableSize();

		};

	}
}