#pragma once
#include "gui/ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

		class Control;

		class IScrollable {

		public:
			IScrollable(Control* control);
			IScrollable(Control* control, float scroll_height);
			IScrollable(Control* control, const Size& scrollable_region);

			virtual void OnScroll(ScrollEventArgs& e);

			void SetVisibleRegion(const Size& region);
			const Size& VisibleRegion() const;
			void SetScrollableRegion(const Size& region);
			const Size& ScrollableRegion() const;

			bool AutoScrollEnabled() const;
			void SetAutoScrollEnabled(bool enabled);

			Point ScrollPosition() const;
			bool HasFocus();

		private:
			Control* _control;
			Size _visible_region;
			Size _scrollable_region;
			bool _auto_scroll_enabled;
			float _vpos;
			float _hpos;

		};

	}

}