#ifndef __IFOCUSABLE_H
#define __IFOCUSABLE_H

class IFocusable {

private:
	static IFocusable* __focused_obj;

public:
	IFocusable();
	// Sets the focus to the object.
	void Focus();
	// Returns whether or not the object currently has focus.
	bool HasFocus();
	// Removes focus from the calling object.
	void BreakFocus();

};

#endif