#ifndef __SCROLLABLE_H
#define __SCROLLABLE_H
#include "IFocusable.h"
#include "gui/Scrollbar.h"

class IScrollable : public IFocusable {
	friend class Gui::Scrollbar;

protected:
	float ScrollPercent;
	float ScrollMin, ScrollMax;

public:
	IScrollable(float scroll_min = 0, float scroll_max = 0) {

		ScrollPercent = 0;
		ScrollMin = scroll_min;
		ScrollMax = scroll_max;

	}
	virtual void Scroll(float percent) {

		ScrollPercent = percent;

	};

};

#endif