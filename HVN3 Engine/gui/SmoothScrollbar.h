#pragma once
#include "Scrollbar.h"

namespace hvn3 {
	namespace Gui {

		class SmoothScrollbar : public Scrollbar {

		public:
			SmoothScrollbar(IScrollable* control, Point position, Size size);
			SmoothScrollbar(IScrollable* control, Point position, Size size, Orientation orientation);

			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;

		protected:
			void _ScrollTargetToPosition() override;

		private:
			float _ease_position;

		};

	}
}