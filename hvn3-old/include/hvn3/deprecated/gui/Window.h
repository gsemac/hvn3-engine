#pragma once
#include <vector>
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui/Panel.h"
#include "hvn3/gui/ITextable.h"
#include "hvn3/io/Mouse.h"

namespace hvn3 {

	namespace Gui {

		class ControlManager;

		class Window : public Control, public ITextable, public IContainer {

		public:
			Window(float x, float y, float width, float height, const char* text);
			~Window();

			void SetTitlebarHeight(float value);
			float TitlebarHeight() const;

			void OnMouseDown(MouseEventArgs& e) override;
			void OnMouseUp(MouseEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseLeave() override;
			void OnClick() override;
			void OnResize(ResizeEventArgs& e) override;
			void OnPaint(PaintEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

		protected:
			const AssetHandle<Graphics::Bitmap>& GetExitIcon();
			PointF ExitButtonPosition() const;
			bool MouseOnExitButton() const;
			void UpdateChildRegion();
			void UpdateDockableRegion();

		private:
			int _titlebar_height;

			AssetHandle<Graphics::Bitmap> _exit_icon;

			PointF _drag_offset;
			bool _dragging, _resizing;
			unsigned int _resizing_side;
			PointF _original_position;
			SizeF _original_size;
			SizeF _size_diff;
			bool _mouse_on_exit_button;
			bool _fade_out;
			//Gui::Panel _panel;

			// Returns the resize regions that the mouse is currently in. Returns 0 if the mouse is not in a resize region.
			unsigned int GetMouseResizeSides() const;
			// Sets the cursor depending on where it is on the Window.
			void SetResizeCursor();
			// Adjusts the size/position of the Window when resizing.
			void HandleResizing();
			bool HasActiveChild() override;

		};

	}

}