#pragma once
#include <vector>
#include "Bitmap.h"
#include "gui/Panel.h"
#include "gui/ITextable.h"
#include "io/Mouse.h"
#include "Graphics.h"

namespace hvn3 {

	namespace Gui {

		class ControlManager;

		class Window : public Control, public ITextable, public IContainer {

		public:
			Window(float x, float y, float width, float height, const char* text);
			~Window();

			void SetTitlebarHeight(float value);
			float TitlebarHeight() const;

			void OnMouseDown() override;
			void OnMouseUp() override;
			void OnMouseMove() override;
			void OnMouseLeave() override;
			void OnClick() override;
			void OnResize(ResizeEventArgs& e) override;
			void OnPaint(PaintEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

		protected:
			const ResourceHandle<Drawing::Bitmap>& GetExitIcon();
			PointF ExitButtonPosition() const;
			bool MouseOnExitButton() const;
			void UpdateChildRegion();

		private:
			int __titlebar_height;

			ResourceHandle<Drawing::Bitmap> _exit_icon;

			PointF __drag_offset;
			bool __dragging, __resizing;
			unsigned int __resizing_side;
			PointF __original_position;
			SizeF __original_size;
			SizeF __size_diff;
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