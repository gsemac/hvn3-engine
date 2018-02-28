#pragma once
#include "hvn3/gui/Control.h"
#include "hvn3/gui/ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

		class IScrollable;

		class Scrollbar : public Control {

		public:
			Scrollbar(IScrollable* control, PointF position, SizeF size);
			Scrollbar(IScrollable* control, PointF position, SizeF size, Orientation orientation);

			void SetTarget(IScrollable* target);
			virtual void SetScrollPercentage(float percent);
			float ScrollPercentage() const;
			void ScrollToBottom();
			void ScrollToTop();

			virtual void OnResize(ResizeEventArgs& e) override;
			virtual void OnMouseEnter() override;
			virtual void OnMouseLeave() override;
			virtual void OnMouseDown(MouseEventArgs& e) override;
			virtual void OnMouseUp(MouseEventArgs& e) override;
			virtual void OnMouseMove(MouseMoveEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnPaint(PaintEventArgs& e) override;

		protected:
			void _setPosition(float position);
			float _getPosition() const;
			float _positionToPercentage(float position) const;
			ScrollEventArgs _getScrollEventArgs(float scroll_percentage) const;
			IScrollable* _getTarget();
			Orientation _getOrientation() const;
			virtual void _scrollTargetToPosition();

		private:
			void _recalculateSliderSize();
			bool _mouseOnSlider();
			int _percentageToPixels(float percentage);
			float _pixelsToPercentage(int pixels);

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