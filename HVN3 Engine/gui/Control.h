#pragma once
#include "IDrawable.h"
#include "IUpdatable.h"
#include "DrawEventArgs.h"
#include "IFocusable.h"
#include "UniquePolymorphicCreateableBase.h"

namespace hvn3 {

	namespace Gui {

		class GuiManager;
		class Control;

		typedef std::unique_ptr<Control> ControlPtr;

		enum ANCHOR {
			ANCHOR_NONE = 0,
			ANCHOR_LEFT = 1,
			ANCHOR_RIGHT = 2,
			ANCHOR_TOP = 4,
			ANCHOR_BOTTOM = 8,
			ANCHOR_ALL = ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM
		};
		//ENABLE_BITFLAG_OPERATORS(ANCHOR);

		enum SIDE {
			TOP = 1,
			BOTTOM = 2,
			LEFT = 4,
			RIGHT = 8
		};
		//ENABLE_BITFLAG_OPERATORS(SIDE);

		enum class Orientation {
			Horizontal = 1,
			Vertical = 2
		};

		enum class DockStyle {
			None,
			Left,
			Right,
			Bottom,
			Top,
			Fill
		};

		struct ResizeEventArgs : public EventArgs {

		public:
			ResizeEventArgs(const Size<float>& old_size, const Size<float>& new_size) :
				_old_size(old_size),
				_new_size(new_size) {
			}

			const Size<float>& OldSize() const {

				return _old_size;

			}
			const Size<float>& NewSize() const {

				return _new_size;

			}

		private:
			Size<float> _old_size;
			Size<float> _new_size;

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
			MoveEventArgs(const Point2d<float>& old_position) :
				_old_position(old_position) {
			}

			const Point2d<float>& OldPosition() const {

				return _old_position;

			}

		private:
			Point2d<float> _old_position;

		};

		class EnabledChangedEventArgs : public EventArgs {

		public:
			EnabledChangedEventArgs(bool enabled) :
				_enabled(enabled) {
			}

			bool Enabled() const {

				return _enabled;

			}

		private:
			bool _enabled;

		};

		class MouseEventArgs : public EventArgs {

		public:
			MouseEventArgs(const PointF& mouse_position) :
				_mouse_position(mouse_position) {
			}

			const PointF& Position() const {

				return _mouse_position;

			}

		private:
			PointF _mouse_position;

		};

		class MouseMoveEventArgs : public MouseEventArgs {

		public:
			MouseMoveEventArgs(const PointF& mouse_position) :
				MouseEventArgs(mouse_position) {
			}

		};

		namespace System {
			class ControlController;
		}

		class Control : public IDrawable, public IUpdatable, public Positionable2dBase<float>, public SizeableBase<float>, public IFocusable, public UniquePolymorphicCreateableBase<Control> {
			friend class System::ControlController;

		public:
			int Z;

			Control();
			Control(const Point2d<float>& location, const hvn3::Size<float>& size);
			virtual ~Control() = default;

			virtual void OnUpdate(UpdateEventArgs& e) override;
			void OnDraw(DrawEventArgs& e) override;
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

			DockStyle Dock() const;
			void SetDock(DockStyle dock);

			float Opacity();
			void SetOpacity(float opacity);

			hvn3::Size<float> MinimumSize();
			void SetMinimumSize(const hvn3::Size<float>& size);
			hvn3::Size<float> MaximumSize();
			void SetMaximumSize(const hvn3::Size<float>& size);

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

			Point2d<float> FixedPosition() const;
			Rectangle<float> Bounds() const;

			float Scale() const;

			bool IsActiveControl();

			void SetX(float x) override;
			void SetY(float y) override;
			void SetPosition(float x, float y) override;

			// Events
			virtual void OnMouseLeave();
			virtual void OnMouseEnter();
			virtual void OnMouseHover();
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseMove(MouseMoveEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
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
			virtual void OnEnabledChanged(EnabledChangedEventArgs& e);

		private:
			bool _disposed;
			bool _invalidated;
			bool _visible;
			bool _enabled;

			Control* _parent;
			GuiManager* __manager;
			Drawing::Bitmap _surface;
			Color __backcolor, __forecolor;
			int _anchor;
			DockStyle _dock;
			float __opacity;
			hvn3::Size<float> __minimum_size;
			hvn3::Size<float> __maximum_size;

			bool __mouse_is_on;
			bool __mouse_is_down;
			Point2d<float> __mouse_last_pos;

			Point2d<float> __previous_pos; // Keeps track of previous position for OnMove event
			bool __prev_focus; // Keeps track of focus state for OnGotFocus/OnLostFocus

			virtual bool HasActiveChild();

			Point2d<float> __fixed_pos;

			Point2d<float> GetFixedPosition() const;
			void ApplyDockStyle();

		};

	}

}