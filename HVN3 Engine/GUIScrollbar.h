#ifndef __GUISCROLLBAR_H
#define __GUISCROLLBAR_H
#include "Object.h"

class IScrollable;

namespace Gui {

	class Scrollbar : public Object {

	private:
		IScrollable* __obj;
		float __width, __height;
		float __slider_height;
		float __position;

		bool __dragging;
		float __mouse_clicked_pos;
		float __starting_position;

		float PositionPercentage();
		void ResizeSlider();
		bool MouseOn();

	public:
		Scrollbar();
		Scrollbar(IScrollable* object, float height);

		void SetHeight(float height);
		void SetTarget(IScrollable* target);
		void SetPosition(float position);
		void ScrollToBottom();
		void ScrollToTop();

		float Width();
		bool Shown();

		void Draw();
		void Update();

	};

}

#endif