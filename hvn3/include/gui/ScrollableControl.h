#pragma once
#include "gui/IScrollable.h"
#include "gui/SmoothScrollbar.h"
#include "gui/Control.h"
#include "gui/Handle.h"

namespace hvn3 {
	namespace Gui {

		class ScrollableControl : public virtual Control, public IScrollable {

			enum SCROLLBAR : bool {
				VERTICAL,
				HORIZONTAL
			};

		public:
			ScrollableControl(float scroll_height);
			ScrollableControl(const SizeF& scrollable_region);
			~ScrollableControl();

			Handle<Scrollbar> VerticalScrollbar();
			Handle<Scrollbar> HorizontalScrollbar();

			bool HasVerticalScrollbar() const;
			bool HasHorizontalScrollbar() const;

			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnPaint(PaintEventArgs& e) override;
			virtual void OnMove(MoveEventArgs& e) override;
			virtual void OnManagerChanged(ManagerChangedEventArgs& e) override;

		private:
			Scrollbar* _scrollbars[2];

			void UpdateScrollbars();
			void UpdateVisibleRegion();

		};

	}
}