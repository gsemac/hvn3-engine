#pragma once
#include "Rectangle.h"

class Scale {

public:
	Scale(float scale);
	Scale(float x_scale, float y_scale);
	Scale(const Rectangle& a, const Rectangle& b);

	float XScale() const;
	float YScale() const;
	Point& ScalePoint(Point& point) const;
	void ScalePoint(float& x, float& y) const;
	Scale Inverse() const;

	friend Scale operator*(const Scale& a, const Scale& b);
	Scale& operator*=(const Scale& other);

private:
	float __scale_factor_x;
	float __scale_factor_y;

};