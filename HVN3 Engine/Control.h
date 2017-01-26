#ifndef __CONTROL_H
#define __CONTROL_H
#include "IDisposable.h"
#include "IFocusable.h"
#include "Object.h"
#include "Color.h"
#include "BitFlags.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

namespace Gui {
	
	enum ANCHOR {
		ANCHOR_NONE = 0x00,
		ANCHOR_LEFT = 0x01,
		ANCHOR_RIGHT = 0x02,
		ANCHOR_TOP = 0x04,
		ANCHOR_BOTTOM = 0x08
	};
	ENABLE_BITFLAG_OPERATORS(ANCHOR);

	enum SIDE {
		TOP = 0x01,
		BOTTOM = 0x02,
		LEFT = 0x04,
		RIGHT = 0x08
	};
	ENABLE_BITFLAG_OPERATORS(SIDE);

	struct EventArgs {};
	struct ResizeEventArgs {
		ResizeEventArgs(float old_width, float old_height, float new_width, float new_height) : OldSize(old_width, old_height), NewSize(new_width, new_height) {

		}

		// The former size of the Control.
		Size OldSize;
		// The new (current) size of the Control.
		Size NewSize;
		// The sides of the Control that have changed position as a result of resizing.
		SIDE ChangedSides;
	};

	class Control : public IDrawable, public IPositionable, public ISizeable, public IDisposable, public IFocusable {
		friend class GuiManager;

	private:
		bool __disposed;
		bool __invalidated;
		bool __visible;
		bool __enabled;
		
		Control* __parent;
		GuiManager* __manager;
		Bitmap __image;
		Drawing::Graphics __graphics;
		Color __backcolor, __forecolor;
		ANCHOR __anchor;
		float __opacity;
		Size __minimum_size;
		Size __maximum_size;

		bool __mouse_is_on;
		bool __mouse_is_down;
		Point __mouse_last_pos;

		Point __previous_pos; // Keeps track of previous position for OnMove event
		bool __prev_focus; // Keeps track of focus state for OnGotFocus/OnLostFocus

		virtual bool HasActiveChild();

		Point __fixed_pos;

		Point GetFixedPosition();

	public:
		int Z;

		Control();
		Control(const Point& location, const Size& size);

		virtual void Update();
		void Draw();
		void Resize(float width, float height);

		virtual void Invalidate();
		bool Invalidated();
		void Dispose();
		bool IsDisposed();

		Color* ForeColor();
		Color* BackColor();
		void SetForeColor(const Color& color);
		void SetBackColor(const Color& color);

		ANCHOR Anchors();
		void SetAnchors(ANCHOR anchors);

		float Opacity();
		void SetOpacity(float opacity);

		Size MinimumSize();
		void SetMinimumSize(const Size& size);
		Size MaximumSize();
		void SetMaximumSize(const Size& size);
		
		Utf8String Tooltip();
		void SetTooltip(const char* text);

		bool Visible();
		void SetVisible(bool is_visible);

		bool Enabled();
		void SetEnabled(bool is_enabled);

		Control* Parent();
		void SetParent(Control* parent);

		GuiManager* Manager();

		void BringToFront();
		void SendToBack();

		Point FixedPosition();
		Rectangle Bounds();

		float Scale() const;

		// Events
		virtual void OnMouseLeave();
		virtual void OnMouseEnter();
		virtual void OnMouseHover();
		virtual void OnMouseDown();
		virtual void OnMouseMove();
		virtual void OnMouseUp();
		virtual void OnClick();
		virtual void OnDoubleClick();
		virtual void OnPaint();
		virtual void OnResize();
		virtual void OnMove();
		virtual void OnGotFocus();
		virtual void OnLostFocus();
		virtual void OnKeyDown();
		virtual void OnKeyPressed();
		virtual void OnKeyReleased();

	};

}

#endif