#include "hvn3/gui2/Window.h"

namespace hvn3 {
	namespace Gui {

		enum EDGE {
			EDGE_TOP = 1,
			EDGE_BOTTOM = 2,
			EDGE_LEFT = 4,
			EDGE_RIGHT = 8
		};



		Window::Window(const String& text) :
			Window(0.0f, 0.0f, 50.0f, 50.0f, text) {
		}
		Window::Window(float x, float y, float width, float height, const String& text) :
			WidgetBase(x, y, width, height),
			_size_before_resize(Size()),
			_minimum_size(Size()),
			_child_manager(this) {

			SetText(text);

			_initializeMembers();

		}
		Window::Window(float width, float height, const String& text) :
			Window(0.0f, 0.0f, width, height, text) {
		}

		bool Window::TitleBarVisible() const {
			return _titlebar_visible;
		}
		void Window::SetTitleBarVisible(bool value) {
			_titlebar_visible = value;
		}
		void Window::SetBorderStyle(BorderStyle value) {
			_border_style = value;
		}
		void Window::Close() {

			if (GetManager() == nullptr)
				return;

			GetManager()->Remove(this);

		}

		WidgetManager& Window::GetChildren() {
			return _child_manager;
		}
		bool Window::HasChildren() {
			return true;
		}

		void Window::OnMousePressed(WidgetMousePressedEventArgs& e) {
			WidgetBase::OnMousePressed(e);

			_mouse_down_position = (e.Position() - Position());
			_resizing_edges = _getHoveredEdges(e.Position());
			_size_before_resize = Size();

			if (_resizing_edges != 0)
				_is_resizing = true;
			else if (DockStyle() == Gui::DockStyle::None)
				_is_dragging = true;

			if (GetManager() != nullptr && DockStyle() == Gui::DockStyle::None)
				GetManager()->BringToFront(this);

		}
		void Window::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {
			WidgetBase::OnMouseReleased(e);

			_is_dragging = false;
			_is_resizing = false;

			_getCursorFromEdges(_getHoveredEdges(e.Position()));

		}
		void Window::OnMouseMove(WidgetMouseMoveEventArgs& e) {
			WidgetBase::OnMouseMove(e);

			if (_is_dragging)
				SetPosition(e.Position() - _mouse_down_position);
			else if (_is_resizing)
				_updateResizing(e.Position());
			else {

				// Get the edges that the mouse is currently hovering over, and set the cursor accordingly.
				int edges = _getHoveredEdges(e.Position());
				SetCursor(_getCursorFromEdges(edges));

			}

		}
		void Window::OnUpdate(WidgetUpdateEventArgs& e) {

			WidgetBase::OnUpdate(e);

			_updateDockableRegion();

		}



		void Window::_initializeMembers() {

			SetIdentifier("window");
			_is_dragging = false;
			_is_resizing = false;
			_resizing_edges = 0;
			_resize_edge_width = 6.0f;
			_titlebar_height = 20.0f;
			_minimum_size = SizeF(_resize_edge_width * 2.0f, _titlebar_height + _resize_edge_width);
			_border_style = BorderStyle::Sizable;

			_titlebar_visible = Text().Length() > 0;

			_updateDockableRegion();

		}
		int Window::_getHoveredEdges(const PointF& p) {

			if (_border_style == BorderStyle::None)
				return 0;

			int side = 0;
			float edge_w = _resize_edge_width;

			if (!HasFlag(Anchor(), Anchor::Top) && Math::Geometry::PointIn(p, RectangleF(X(), Y(), Width(), edge_w)))
				side |= EDGE_TOP;
			else if (!HasFlag(Anchor(), Anchor::Bottom) && Math::Geometry::PointIn(p, RectangleF(X(), Y() + Height() - edge_w, Width(), edge_w)))
				side |= EDGE_BOTTOM;

			if (!HasFlag(Anchor(), Anchor::Left) && Math::Geometry::PointIn(p, RectangleF(X(), Y(), edge_w, Height())))
				side |= EDGE_LEFT;
			else if (!HasFlag(Anchor(), Anchor::Right) && Math::Geometry::PointIn(p, RectangleF(X() + Width() - edge_w, Y(), edge_w, Height())))
				side |= EDGE_RIGHT;

			return side;

		}
		SystemCursor Window::_getCursorFromEdges(int edges) {

			if (edges & EDGE_LEFT) {
				if (edges & EDGE_TOP)
					return SystemCursor::ResizeNW;
				else if (edges & EDGE_BOTTOM)
					return SystemCursor::ResizeSW;
				return SystemCursor::ResizeW;
			}
			else if (edges & EDGE_RIGHT) {
				if (edges & EDGE_TOP)
					return SystemCursor::ResizeNE;
				else if (edges & EDGE_BOTTOM)
					return SystemCursor::ResizeSE;
				return SystemCursor::ResizeE;
			}
			else if (edges & EDGE_TOP)
				return SystemCursor::ResizeN;
			else if (edges & EDGE_BOTTOM)
				return SystemCursor::ResizeS;

			return SystemCursor::Default;

		}
		void Window::_updateResizing(const PointF& mouse_position) {

			int edges = _resizing_edges;

			PointF diff = (_mouse_down_position - (mouse_position - Position()));
			SizeF new_size = Size();

			if (edges & EDGE_RIGHT)
				new_size.SetWidth(_size_before_resize.width - diff.x);
			else if (edges & EDGE_LEFT) {
				new_size.SetWidth(_size_before_resize.width + diff.x);
				SetX(X() - (new_size.width - _size_before_resize.width));
				_size_before_resize.width = new_size.width;
			}

			if (edges & EDGE_BOTTOM)
				new_size.SetHeight(_size_before_resize.height - diff.y);
			else if (edges & EDGE_TOP) {
				new_size.SetHeight(_size_before_resize.height + diff.y);
				SetY(Y() - (new_size.height - _size_before_resize.height));
				_size_before_resize.height = new_size.height;
			}

			if (new_size.width < _minimum_size.width)
				new_size.width = _minimum_size.width;

			if (new_size.height < _minimum_size.height)
				new_size.height = _minimum_size.height;

			SetSize(new_size);

		}
		void Window::_updateDockableRegion() {

			float edge_w;

			if (_border_style == BorderStyle::None)
				edge_w = 0.0f;
			else
				edge_w = _resize_edge_width;

			float dx = 0.0f;
			float dy = 0.0f;
			float dw = Width();
			float dh = Height();

			if (!HasFlag(Anchor(), Anchor::Left)) {
				dx = edge_w;
				dw -= edge_w;
			}

			if (!HasFlag(Anchor(), Anchor::Right))
				dw -= edge_w;

			if (_titlebar_visible) {
				dy = _titlebar_height;
				dh -= _titlebar_height;
			}
			else if (!HasFlag(Anchor(), Anchor::Top)) {
				dy = edge_w;
				dh -= edge_w;
			}

			if (!HasFlag(Anchor(), Anchor::Bottom))
				dh -= edge_w;

			_child_manager.SetDockableRegion(RectangleF(dx, dy, dw, dh));

		}

	}
}