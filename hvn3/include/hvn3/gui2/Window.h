#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Window : public WidgetBase {
			
		public:
			Window(const String& text);
			Window(float x, float y, float width, float height, const String& text);
			Window(float width, float height, const String& text);

			bool TitleBarVisible() const;
			void SetTitleBarVisible(bool value);
			void Close();

			WidgetManager& GetChildren() override;
			bool HasChildren() override;

			void OnMousePressed(WidgetMousePressedEventArgs& e) override;
			void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;
			void OnMouseMove(WidgetMouseMoveEventArgs& e) override;
			void OnUpdate(WidgetUpdateEventArgs& e) override;

		private:
			PointF _mouse_down_position;
			bool _is_dragging;
			bool _is_resizing;
			int _resizing_edges;
			SizeF _size_before_resize;
			float _resize_edge_width;
			float _titlebar_height;
			bool _titlebar_visible;
			SizeF _minimum_size;
			WidgetManager _child_manager;

			void _initializeMembers();
			int _getHoveredEdges(const PointF& mouse_position);
			SystemCursor _getCursorFromEdges(int edges);
			void _updateResizing(const PointF& mouse_position);
			void _updateDockableRegion();

		};

	}
}