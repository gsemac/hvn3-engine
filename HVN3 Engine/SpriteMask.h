#pragma once
#include <vector>
#include "Geometry.h"
#include "Sprite.h"

class SpriteMask {

private:
	float __degrees;
	std::vector<Rectangle> __strips;
	void GenerateMask(Sprite* sprite);

public:
	SpriteMask();
	SpriteMask(Sprite* sprite);

	float Angle();
	std::vector<Rectangle>& Strips();

	void Rotate(Point origin, float degrees, bool relative = true);
	void Translate(float x_offset, float y_offset);
	void Scale(float x_scale, float y_scale);
	void Move(float x, float y);

	bool Intersects(const Circle& other) const;
	bool Intersects(const Rectangle& other) const;
	bool Intersects(const Line& other) const;

	Rectangle& operator[](const size_t& strip);

};