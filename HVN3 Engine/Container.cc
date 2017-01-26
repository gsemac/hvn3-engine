//#include "Container.h"
//
//namespace Gui {
//
//	// Private members
//
//	bool Container::HasActiveChild() {
//
//		return (__manager.ActiveControl() != nullptr);
//
//	}
//
//	// Public members
//
//	Container::Container(float x, float y, float width, float height) : 
//		Control(Point(x, y), Size(width, height)),
//		__prev_size(width, height),
//		__prev_pos(x, y) {
//
//
//
//	}
//
//	void Container::AddControl(Control* control) {
//
//		// Adjust the position of the Control to be relative to that of the Container.
//		control->Translate(X(), Y());
//
//		// Add the Control to the manager.
//		__manager.AddControl(control);
//
//	}
//	void Container::RemoveControl(Control*& control) {
//
//		__manager.RemoveControl(control);
//
//	}
//	std::list<Control*>& Container::Controls() {
//
//		return __manager.Controls();
//
//	}
//
//}