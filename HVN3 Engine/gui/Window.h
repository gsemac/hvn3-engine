#pragma once
#include <vector>
#include "Bitmap.h"
#include "gui/Panel.h"
#include "gui/ITextable.h"
#include "io/Mouse.h"
#include "Graphics.h"

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
		void OnResize() override;
		void OnPaint(PaintEventArgs& e) override;
		void Update(UpdateEventArgs& e) override;

	protected:
		const ResourceHandle<Drawing::Bitmap>& GetExitIcon();
		Point ExitButtonPosition() const;
		bool MouseOnExitButton() const;
		void UpdateChildRegion();

	private:
		int __titlebar_height;

		ResourceHandle<Drawing::Bitmap> _exit_icon;

		Point __drag_offset;
		bool __dragging, __resizing;
		unsigned int __resizing_side;
		Point __original_position;
		Size __original_size;
		Size __size_diff;
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