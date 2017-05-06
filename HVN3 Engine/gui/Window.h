#pragma once
#include <vector>
#include "Bitmap.h"
#include "Container.h"
#include "Panel.h"
#include "io/Mouse.h"
#include "Graphics.h"
#include "ITextable.h"

namespace Gui {

	class Window : public Control, public ITextable {

	public:
		Window(float x, float y, float width, float height, const char* text);
		~Window();

		void AddControl(Control* control);
		void RemoveControl(Control* control);
		std::list<Control*>& Controls();
		void SetTitlebarHeight(float value);
		float TitlebarHeight() const;

		void OnMouseDown() override;
		void OnMouseUp() override;
		void OnMouseMove() override;
		void OnMouseLeave() override;
		void OnResize() override;
		void OnPaint(PaintEventArgs& e) override;
		void Update(UpdateEventArgs& e) override;

	protected:
		const ResourceHandle<Drawing::Bitmap>& GetExitIcon();

	private:
		int __titlebar_height;

		ResourceHandle<Drawing::Bitmap> _exit_icon;

		Point __drag_offset;
		bool __dragging, __resizing;
		unsigned int __resizing_side;
		Point __original_position;
		Size __original_size;
		Size __size_diff;
		Panel __panel;

		// Returns the resize regions that the mouse is currently in. Returns 0 if the mouse is not in a resize region.
		unsigned int GetMouseResizeRegions();
		// Sets the cursor depending on where it is on the Window.
		void SetResizeCursor();
		// Adjusts the size/position of the Window when resizing.
		void HandleResizing();
		bool HasActiveChild() override;

	};

}