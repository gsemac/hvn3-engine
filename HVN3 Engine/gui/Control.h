#pragma once
#include "DrawEventArgs.h"
#include "IFocusable.h"

namespace Gui {

	class GuiManager;
	class Control;

	typedef std::unique_ptr<Control> ControlPtr;

	enum ANCHOR {
		ANCHOR_NONE = 0x00,
		ANCHOR_LEFT = 0x01,
		ANCHOR_RIGHT = 0x02,
		ANCHOR_TOP = 0x04,
		ANCHOR_BOTTOM = 0x08
	};
	//ENABLE_BITFLAG_OPERATORS(ANCHOR);

	enum SIDE {
		TOP = 0x01,
		BOTTOM = 0x02,
		LEFT = 0x04,
		RIGHT = 0x08
	};
	//ENABLE_BITFLAG_OPERATORS(SIDE);

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

	class PaintEventArgs : public DrawEventArgs {

	public:
		using DrawEventArgs::DrawEventArgs;

	};

	class Control : public IDrawable, public IUpdatable, public IPositionable, public ISizeable, public IFocusable {
		friend class ControlController;

	public:
		int Z;

		Control();
		Control(const Point& location, const Size& size);

		virtual void Update(UpdateEventArgs& e) override;
		void Draw(DrawEventArgs& e) override;
		void Resize(float width, float height);

		virtual void Invalidate();
		bool Invalidated();
		void Dispose();
		bool IsDisposed();

		const Color& ForeColor() const;
		const Color& BackColor() const;
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
		
		String Tooltip();
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
		virtual void OnPaint(PaintEventArgs& e);
		virtual void OnResize();
		virtual void OnMove();
		virtual void OnGotFocus();
		virtual void OnLostFocus();
		virtual void OnKeyDown();
		virtual void OnKeyPressed();
		virtual void OnKeyReleased();

		template<typename T, typename ... Args>
		static std::unique_ptr<Control> Create(Args &&... args) {

			return std::make_unique<T>(std::forward<Args>(args)...);

		}

		template<typename T>
		static std::unique_ptr<Control> Create(T* ptr) {

			return std::unique_ptr<T>(ptr);

		}

	private:
		bool __disposed;
		bool _invalidated;
		bool __visible;
		bool __enabled;

		Control* _parent;
		GuiManager* __manager;
		Drawing::Bitmap _surface;
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

	};

}