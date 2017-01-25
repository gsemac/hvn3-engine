#ifndef __VIEW_H
#define __VIEW_H
#include "Object.h"

class View {

public:
	View(Point view_position, Size view_size, Point port_position, Size port_size);
	View(Point view_position, Size view_size, Point port_position, Size port_size, Object* follow_obj, float horizontal_border, float vertical_border);

	void SetFollowing(Object* obj);
	Object* GetFollowing();

	Rectangle Region() const;
	Rectangle Port() const;
	Point& ViewPosition();
	float ViewX() const;
	float ViewY() const;
	float ScaleX() const;
	float ScaleY() const;

	float HorizontalBorder() const;
	float VerticalBorder() const;

	float Angle() const;
	void SetAngle(float angle);

	void Enable();
	void Disable();
	bool Enabled() const;

	bool HasMouse() const;
	Point MousePosition() const;
	bool TracksMouse() const;
	void TracksMouse(bool tracks);

private:
	Point __view_pos;
	Size __view_size;
	Point __port_pos;
	Size __port_size;

	Object* __follow_obj;
	float __hbor, __vbor, __vspd, __hspd;
	float __angle;
	bool __enabled;
	bool __tracks_mouse;

};

#endif