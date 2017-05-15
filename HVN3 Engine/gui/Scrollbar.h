#pragma once
#include "gui/Control.h"

namespace Gui {

	class IScrollable;

	class Scrollbar : public Control {
		
	public:
		Scrollbar(IScrollable* control, Point position, Size size);
		Scrollbar(IScrollable* control, Point position, Size size, Orientation orientation);

		void SetTarget(IScrollable* target);
		void SetScrollPercentage(float percent);
		float ScrollPercentage() const;
		void ScrollToBottom();
		void ScrollToTop();

		void OnResize() override;
		void Update(UpdateEventArgs& e) override;
		void OnPaint(PaintEventArgs& e) override;

	private:
		IScrollable* _target;
		Orientation _orientation;
		float _position;

		float _slider_height;

		bool _dragging;
		float _mouse_clicked_pos;
		float _starting_position;

		void RecalculateSliderSize();
		bool MouseOnSlider();
		void ScrollTargetToPosition();

	};

}