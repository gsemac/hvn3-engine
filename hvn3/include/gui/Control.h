#pragma once
#include "core/IDrawable.h"
#include "core/IUpdatable.h"
#include "core/DrawEventArgs.h"
#include "core/UniquePolymorphicCreateableBase.h"
#include "gui/IFocusable.h"
#include "utility/UTF8String.h"

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

		struct ResizeEventArgs : public hvn3::System::EventArgs {

		public:
			ResizeEventArgs(const SizeF& old_size, const SizeF& new_size) :
				_old_size(old_size),
				_new_size(new_size) {
			}

			const SizeF& OldSize() const {

				return _old_size;

			}
			const SizeF& NewSize() const {

				return _new_size;

			}

		private:
			SizeF _old_size;
			SizeF _new_size;

		};

		class PaintEventArgs : public DrawEventArgs {

		public:
			using DrawEventArgs::DrawEventArgs;

		};

		class ManagerChangedEventArgs : public hvn3::System::EventArgs {

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

		class MoveEventArgs : public hvn3::System::EventArgs {

		public:
			MoveEventArgs(const PointF& old_position) :
				_old_position(old_position) {
			}

			const PointF& OldPosition() const {

				return _old_position;

			}

		private:
			PointF _old_position;

		};

		class EnabledChangedEventArgs : public hvn3::System::EventArgs {

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

		class MouseEventArgs : public hvn3::System::EventArgs {

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

		class Control : public IDrawable, public IUpdatable, public Positionable2dBase<float>, public SizeableBase<float>, public IFocusable {
			friend class System::ControlController;

		public:
			int Z;

			Control();
			Control(const PointF& location, const hvn3::SizeF& size);
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

			hvn3::SizeF MinimumSize();
			void SetMinimumSize(const hvn3::SizeF& size);
			hvn3::SizeF MaximumSize();
			void SetMaximumSize(const hvn3::SizeF& size);

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

			PointF FixedPosition() const;
			RectangleF Bounds() const;

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
			GuiManager* _manager;
			Graphics::Bitmap _surface;
			Color _backcolor, _forecolor;
			int _anchor;
			DockStyle _dock;
			float _opacity;
			hvn3::SizeF _minimum_size;
			hvn3::SizeF _maximum_size;

			bool _mouse_is_on;
			bool _mouse_is_down;
			PointF _mouse_last_pos;

			PointF _previous_pos; // Keeps track of previous position for OnMove event
			bool _prev_focus; // Keeps track of focus state for OnGotFocus/OnLostFocus

			virtual bool HasActiveChild();

			PointF _fixed_pos;

			PointF GetFixedPosition() const;
			void ApplyDockStyle();

		};

	}

}