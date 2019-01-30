#pragma once
#include "hvn3/gui2/Scrollbar.h"
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class ScrollableWidgetBase :
			public virtual WidgetBase {

		public:
			ScrollableWidgetBase() :
				ScrollableWidgetBase(SizeF(0.0f, 0.0f)) {}
			ScrollableWidgetBase(const SizeF& scrollable_size) :
				_scrollable_size(scrollable_size) {
			}

			void SetVisible(bool value) override {

				WidgetBase::SetVisible(value);

				if (_scrollbarsCreated()) {
					_vscrollbar->SetVisible(value);
					_hscrollbar->SetVisible(value);
				}

			}

			void OnManagerChanged(WidgetManagerChangedEventArgs& e) override {

				if (GetManager() == nullptr) {

					// If the new manager is null, destroy both scrollbars (if they exist).
					_destroyScrollbars(e.OldManager());
					return;
				}
				else if (_scrollbarsCreated()) {

					// If the new manager is NOT null and scrollbars have already been created, move them to the new manager.
					_moveScrollbars(e.OldManager(), GetManager());
					return;

				}

				// If the scrollbars don't already exist, create them and add them to the new manager.
				_createScrollbars();

			}
			void OnUpdate(WidgetUpdateEventArgs& e) override {

				if (!_scrollbarsCreated())
					return;

				PointF pos = Position();

				// Make sure the scrollbars stick to their positions.
				_vscrollbar->SetPosition(pos.X() + Width() - _vscrollbar->Width(), pos.Y());
				_hscrollbar->SetPosition(pos.X(), pos.Y() + Height() - _hscrollbar->Height());

				// Only have the scrollbars visible if they're required to view the scrollable area.
				_vscrollbar->SetVisible(Height() < _scrollable_size.height);
				_hscrollbar->SetVisible(Width() < _scrollable_size.width);

				// Make sure the scrollbars match the widget size.

				_vscrollbar->SetLength(Height() - (_hscrollbar->Visible() ? _hscrollbar->Height() : 0.0f));
				_hscrollbar->SetLength(Width() - (_vscrollbar->Visible() ? _vscrollbar->Width() : 0.0f));

				_vscrollbar->SetMax(_scrollable_size.height);
				_hscrollbar->SetMax(_scrollable_size.width);

			}

		protected:
			Scrollbar* VerticalScrollbar() {
				return _vscrollbar;
			}
			Scrollbar* HorizontalScrollbar() {
				return _hscrollbar;
			}
			RectangleF VisibleRegion() const {

				RectangleF region(Size());

				if (_hscrollbar != nullptr && _hscrollbar->Visible()) {

					float wdiff = _scrollable_size.width - Width();
					float sdiff = _vscrollbar->Visible() ? _vscrollbar->Width() : 0.0f;

					region.SetX((wdiff + sdiff) * _hscrollbar->ValuePercent());

				}
				if (_vscrollbar != nullptr && _vscrollbar->Visible()) {

					float hdiff = _scrollable_size.height - Height();
					float sdiff = _hscrollbar->Visible() ? _hscrollbar->Height() : 0.0f;

					region.SetY((hdiff + sdiff) * _vscrollbar->ValuePercent());

				}

				return region;

			}
			void SetSmoothScrollEnabled(bool value) {

				_smooth_scroll_enabled = value;

				if (_scrollbarsCreated()) {
					VerticalScrollbar()->SetSmoothScrollEnabled(value);
					HorizontalScrollbar()->SetSmoothScrollEnabled(value);
				}

			}
			const SizeF& ScrollableSize() const {

				return _scrollable_size;

			}
			void SetScrollableSize(const SizeF& scrollable_size) {
				_scrollable_size = scrollable_size;
			}

		private:
			Scrollbar* _vscrollbar;
			Scrollbar* _hscrollbar;
			SizeF _scrollable_size;
			bool _smooth_scroll_enabled;

			void _createScrollbars() {

				_vscrollbar = new Scrollbar(Height(), 1.0f, Orientation::Vertical);
				_hscrollbar = new Scrollbar(Width(), 1.0f, Orientation::Horizontal);

				GetManager()->Add(_vscrollbar);
				GetManager()->Add(_hscrollbar);

				_vscrollbar->BringToFront();
				_hscrollbar->BringToFront();

				SetSmoothScrollEnabled(_smooth_scroll_enabled);

			}
			bool _scrollbarsCreated() const {
				return _vscrollbar != nullptr && _hscrollbar != nullptr;
			}
			void _moveScrollbars(WidgetManager* from, WidgetManager* to) {

				if (_vscrollbar != nullptr)
					from->Move(_vscrollbar, to);
				if (_hscrollbar != nullptr)
					from->Move(_hscrollbar, to);

			}
			void _destroyScrollbars(WidgetManager* manager) {

				if (manager != nullptr) {
					if (_vscrollbar != nullptr)
						manager->Remove(_vscrollbar);
					if (_hscrollbar != nullptr)
						manager->Remove(_hscrollbar);
				}

				_vscrollbar = nullptr;
				_hscrollbar = nullptr;

			}

		};

	}
}