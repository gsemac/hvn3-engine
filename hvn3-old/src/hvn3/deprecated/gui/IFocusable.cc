#include "hvn3/gui/IFocusable.h"

namespace hvn3 {

	IFocusable* IFocusable::__focused_obj = nullptr;
	IFocusable::IFocusable() {}
	void IFocusable::Focus() { __focused_obj = this; }
	bool IFocusable::HasFocus() { return __focused_obj == this; }
	void IFocusable::BreakFocus() {

		if (__focused_obj == this)
			__focused_obj = nullptr;

	}

}