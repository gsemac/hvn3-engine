#pragma once
#include "DrawEventArgs.h"
#include "IFocusable.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

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

		enum class Orientation {
			Horizontal = 1,
			Vertical = 2
		};

		struct ResizeEventArgs : public EventArgs {

		public:
			ResizeEventArgs(const Size& old_size, const Size& new_size) :
				_old_size(old_size),
				_new_size(new_size) {
			}

			const Size& OldSize() const {

				return _old_size;

			}
			const Size& NewSize() const {

				return _new_size;

			}

		private:
			Size _old_size;
			Size _new_size;

		};

		class PaintEventArgs : public DrawEventArgs {

		public:
			using DrawEventArgs::DrawEventArgs;

		};

		class ManagerChangedEventArgs : public EventArgs {

		public:
			ManagerChangedEventArgs(GuiManager* old_manager) {

				_old_manager = old_manager;

			}
			GuiManager* PreviousManager() {

				return _old_manager;

			}

		private:
			GuiManager* _old_manager;

		};

		class MoveEventArgs : public EventArgs {

		public:
			MoveEventArgs(const Point& old_position) :
				_old_position(old_position) {
			}

			const Point& OldPosition() const {

				return _old_position;

			}

		private:
			Point _old_position;

		};

		class Control : public IDrawable, public IUpdatable, public IPositionable, public ISizeable, public IFocusable, public UniqueCreateableBase<Control> {
			friend class ControlController;

		public:
			int Z;

			Control();
			Control(const Point& location, const Size& size);
			virtual ~Control() = default;

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

			int Anchors();
			void SetAnchors(int anchors);

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
			const Control* Parent() const;
			void SetParent(Control* parent);

			GuiManager* Manager();
			const GuiManager* Manager() const;

			void BringToFront();
			void SendToBack();

			Point FixedPosition() const;
			Rectangle Bounds() const;

			float Scale() const;

			bool IsActiveControl();

			void SetX(float x) override;
			void SetY(float y) override;
			void SetXY(float x, float y) override;

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
			virtual void OnResize(ResizeEventArgs& e);
			virtual void OnMove(MoveEventArgs& e);
			virtual void OnGotFocus();
			virtual void OnLostFocus();
			virtual void OnKeyDown();
			virtual void OnKeyPressed();
			virtual void OnKeyReleased();
			virtual void OnManagerChanged(ManagerChangedEventArgs& e);

		private:
			bool __disposed;
			bool _invalidated;
			bool __visible;
			bool __enabled;

			Control* _parent;
			GuiManager* __manager;
			Drawing::Bitmap _surface;
			Color __backcolor, __forecolor;
			int __anchor;
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

			Point GetFixedPosition() const;

		};

	}

}