#pragma once
#include "hvn3/gui/ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

		class Control;

		class IScrollable {

		public:
			IScrollable(Control* control);
			IScrollable(Control* control, float scroll_height);
			IScrollable(Control* control, const Size<float>& scrollable_region);

			virtual void OnScroll(ScrollEventArgs& e);

			void SetVisibleRegion(const Size<float>& region);
			const Size<float>& VisibleRegion() const;
			void SetScrollableRegion(const Size<float>& region);
			const Size<float>& ScrollableRegion() const;

			bool AutoScrollEnabled() const;
			void SetAutoScrollEnabled(bool enabled);

			Point2d<float> ScrollPosition() const;
			bool HasFocus();

		private:
			Control* _control;
			Size<float> _visible_region;
			Size<float> _scrollable_region;
			bool _auto_scroll_enabled;
			float _vpos;
			float _hpos;

		};

	}

}