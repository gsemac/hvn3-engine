#pragma once
#include "hvn3/gui/Scrollbar.h"

namespace hvn3 {
	namespace Gui {

		class SmoothScrollbar : public Scrollbar {

		public:
			SmoothScrollbar(IScrollable* control, PointF position, SizeF size);
			SmoothScrollbar(IScrollable* control, PointF position, SizeF size, Orientation orientation);

			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;

		protected:
			void _scrollTargetToPosition() override;

		private:
			float _ease_position;

		};

	}
}