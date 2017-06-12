#pragma once
#include "IContainer.h"
#include "IScrollable.h"

namespace hvn3 {

	namespace Gui {

		class Scrollbar;

		class Panel : public Control, public IContainer, public IScrollable {

			enum SCROLLBAR : bool {
				VERTICAL,
				HORIZONTAL
			};

		public:
			Panel(const Point2F& position, const SizeF& dimensions);
			Panel(const Point2F& position, const SizeF& dimensions, const SizeF& scrollable_region);
			virtual ~Panel();

			virtual void OnScroll(ScrollEventArgs& e) override;
			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnPaint(PaintEventArgs& e) override;
			virtual void OnMove(MoveEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnManagerChanged(ManagerChangedEventArgs& e) override;
			virtual void OnClick() override;

		private:
			Scrollbar* _scrollbars[2];

			bool HasActiveChild() override;

			void UpdateScrollbarPositionsAndSizes();
			Scrollbar* HorizontalScrollbar();
			Scrollbar* VerticalScrollbar();
			SizeF RecalculateVisibleRegion();
			RectangleF GetControlBounds();

		};

	}

}