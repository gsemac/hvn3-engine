#pragma once
#include "gui/Control.h"
#include "gui/ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

		class IScrollable;

		class Scrollbar : public Control {

		public:
			Scrollbar(IScrollable* control, Point position, Size size);
			Scrollbar(IScrollable* control, Point position, Size size, Orientation orientation);

			void SetTarget(IScrollable* target);
			virtual void SetScrollPercentage(float percent);
			float ScrollPercentage() const;
			void ScrollToBottom();
			void ScrollToTop();

			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown() override;
			virtual void OnMouseUp() override;
			virtual void OnMouseMove() override;
			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnPaint(PaintEventArgs& e) override;

		protected:
			void _SetPosition(float position);
			float _Position() const;
			float _PositionToPercentage(float position) const;
			ScrollEventArgs _GetScrollEventArgs(float scroll_percentage) const;
			IScrollable* _Target();
			Orientation _Orientation() const;
			virtual void _ScrollTargetToPosition();

		private:
			void _RecalculateSliderSize();
			bool _MouseOnSlider();
			int _PercentageToPixels(float percentage);
			float _PixelsToPercentage(int pixels);

			IScrollable* _target;
			Orientation _orientation;
			float _position;

			float _slider_height;

			bool _dragging;
			float _mouse_clicked_pos;
			float _starting_position;

		};

	}

}