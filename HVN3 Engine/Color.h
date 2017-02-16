#pragma once
#include <allegro5/allegro.h>
#include "Helper.h"

class Color {

public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b, float a = 1.0f);
	Color(ALLEGRO_COLOR color);
	Color(const Color& color);

	static Color FromArgb(unsigned char r, unsigned char g, unsigned char b, float a = 1.0f);
	static Color FromArgbf(float r, float g, float b, float a = 1.0f);
	static Color FromHsl(float h, float s, float l, float a = 1.0f);
	static Color FromHex(unsigned long hex);

	static const Color Black;
	static const Color DarkGrey;
	static const Color DimGrey;
	static const Color DodgerBlue;
	static const Color Gainsboro;
	static const Color Grey;
	static const Color LtGrey;
	static const Color Red;
	static const Color Silver;
	static const Color SlateGrey;
	static const Color White;
	static const Color Transparent;

	unsigned char R() const;
	unsigned char G() const;
	unsigned char B() const;
	unsigned char Alpha() const;
	float Rf() const;
	float Gf() const;
	float Bf() const;
	float Alphaf() const;

	float Luminance() const;
	float Hue() const;
	float Saturation() const;

	Color Lighter(float factor = 0.05f) const;
	Color Darker(float factor = 0.05f) const;

	ALLEGRO_COLOR AlPtr() const;

	static bool IsTransparent(ALLEGRO_COLOR& color);

private:
	float __r, __g, __b, __a;

	void RgbToHsl();
	void HslToRgb(float h, float s, float l);

};