#include <algorithm>
#include <cmath>
#include "Color.h"

// Public member functions

Color::Color() : Color(0, 0, 0) {}
Color::Color(unsigned char r, unsigned char g, unsigned char b, float a) {

	__r = (r / 255.0f);
	__g = (g / 255.0f);
	__b = (b / 255.0f);
	__a = a;

}
Color::Color(ALLEGRO_COLOR color) : Color(color.r, color.b, color.b, color.a) {

}
Color::Color(const Color& color) : Color(color.R(), color.G(), color.B()) {}

Color Color::FromArgb(unsigned char r, unsigned char g, unsigned char b, float a) {

	return Color(r, g, b, a);

}
Color Color::FromArgbf(float r, float g, float b, float a) {

	Color c;
	c.__r = r;
	c.__g = g;
	c.__b = b;
	c.__a = a;
	return c;

}
Color Color::FromHsl(float h, float s, float l, float a) {

	Color c;
	c.HslToRgb(h, s, l);
	c.__a = a;
	return c;

}
Color Color::FromHex(unsigned long hex) {

	float r = (hex >> 16) & 0xFF;
	float g = (hex >> 8) & 0xFF;
	float b = hex & 0xFF;
	return Color(r, g, b);

}

unsigned char Color::R() const {

	return (std::round)(__r * 255.0f);

}
unsigned char Color::G() const {

	return (std::round)(__g * 255.0f);

}
unsigned char Color::B() const {

	return (std::round)(__b * 255.0f);

}
float Color::Rf() const {

	return __r;

}
float Color::Gf() const {

	return __g;

}
float Color::Bf() const {

	return __b;

}

unsigned char Color::Alpha() const {

	return (std::round)(__a * 255.0f);

}
float Color::Alphaf() const {

	return __a;

}
float Color::Luminance() const {

	// Calculate luminance.
	float max = (std::max)({ __r, __g, __b });
	float min = (std::min)({ __r, __g, __b });
	float l = (max + min) / 2.0f;

	// Return the result.
	return l;

}
float Color::Hue() const {

	// Get luminance and saturation to use in calculation.
	float l = Luminance();
	float s = Saturation();

	// Calculate the min and max of the RGB values.
	float max = (std::max)({ __r, __g, __b });
	float min = (std::min)({ __r, __g, __b });

	// Calculate hue.
	float h;
	if (s > 0.0f) {
		if (__r > __g && __r > __b)
			h = (__g - __b) / (max - min);
		else if (__g > __r && __g > __b)
			h = 2.0f + (__b - __r) / (max - min);
		else
			h = 4.0f + (__r - __g) / (max - min);
	}
	else
		h = 0.0f;

	// Express hue in degrees.
	h *= 60.0f;
	if (h < 0.0f) h += 360.0f;

	// Return the result.
	return h;

}
float Color::Saturation() const {

	// Get luminance to use in calculation.
	float l = Luminance();

	// Calculate the min and max of the RGB values.
	float max = (std::max)({ __r, __g, __b });
	float min = (std::min)({ __r, __g, __b });

	// Get the denominator to be used in the calculation.
	float d;
	if (l <= 0.5f)
		d = max + min;
	else
		d = 2.0f - max - min;

	// If the denominator is 0, saturation is 0.
	if (d == 0.0f)
		return 0.0f;

	// Calculate saturation.
	return (max - min) / d;

}

Color Color::Lighter(float factor) const {

	// Increase luminance.
	float h = Hue();
	float s = Saturation();
	float l = Clamp(Luminance() + factor, 0.0f, 1.0f);

	// Recalculate RGB values.
	return Color::FromHsl(h, s, l);

}
Color Color::Darker(float factor) const {

	// Decrease luminance.
	float h = Hue();
	float s = Saturation();
	float l = Clamp(Luminance() - factor, 0.0f, 1.0f);

	// Recalculate RGB values.
	return Color::FromHsl(h, s, l);

}

ALLEGRO_COLOR Color::AlPtr() const {

	return al_map_rgba_f(__r, __g, __b, __a);

}

bool Color::IsTransparent(ALLEGRO_COLOR& color) {

	return !(color.a > 0.0f);

}

// Private member functions

void Color::RgbToHsl() {

	// Logic taken from here: http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/

	float h, s, l;

	float max = (std::max)({ __r, __g, __b });
	float min = (std::min)({ __r, __g, __b });

	// Calculate luminance.
	l = (max + min) / 2.0f;

	// Calculate saturation.
	if (l < 0.5f)
		s = (max - min) / (max + min);
	else if (l > 0.5f)
		s = (max - min) / (2.0f - max - min);
	else
		s = 0.0f;

	// Calculate hue.
	if (s > 0.0f) {
		if (__r > __g && __r > __b)
			h = (__g - __b) / (max - min);
		else if (__g > __r && __g > __b)
			h = 2.0f + (__b - __r) / (max - min);
		else
			h = 4.0f + (__r - __g) / (max - min);
	}
	else {
		h = 0.0f;
	}

	// Convert hue to degrees.
	h *= 60.0f;
	if (h < 0.0f)
		h += 360.0f;

}
void Color::HslToRgb(float h, float s, float l) {

	// Logic taken from here: http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/

	// No saturation means the color is a shade of grey.
	if (s == 0.0f) {

		__r = l;
		__g = l;
		__b = l;

	}
	else {

		// Calculate temporary variables based on luminance.
		float temporary_1;
		float temporary_2;
		if (l < 0.5f)
			temporary_1 = l * (1.0f + s);
		else
			temporary_1 = (l + s) - (l * s);
		temporary_2 = 2.0f * l - temporary_1;

		// Convert hue from degrees.
		float hue = h / 360.0f;

		// Calculate temporary RGB values.
		float temporary_R = hue + (1.0f / 3.0f);
		float temporary_G = hue;
		float temporary_B = hue - (1.0f / 3.0f);

		// Make sure each value is in the range 0.0 - 1.0.
		if (temporary_R < 0.0f)
			temporary_R += 1.0f;
		else if (temporary_R > 1.0f)
			temporary_R -= 1.0f;

		if (temporary_G < 0.0f)
			temporary_G += 1.0f;
		else if (temporary_G > 1.0f)
			temporary_G -= 1.0f;

		if (temporary_B < 0.0f)
			temporary_B += 1.0f;
		else if (temporary_B > 1.0f)
			temporary_B -= 1.0f;

		// Calculate R.
		if (6.0f * temporary_R < 1.0f)
			__r = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_R;
		else if (2.0f * temporary_R < 1.0f)
			__r = temporary_1;
		else if (3.0f * temporary_R < 2.0f)
			__r = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_R) * 6.0f;
		else
			__r = temporary_2;

		// Calculate G.
		if (6.0f * temporary_G < 1.0f)
			__g = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_G;
		else if (2.0f * temporary_G < 1.0f)
			__g = temporary_1;
		else if (3.0f * temporary_G < 2.0f)
			__g = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_G) * 6.0f;
		else
			__g = temporary_2;

		// Calculate B.
		if (6.0f * temporary_B < 1.0f)
			__b = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_B;
		else if (2.0f * temporary_B < 1.0f)
			__b = temporary_1;
		else if (3.0f * temporary_B < 2.0f)
			__b = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_B) * 6.0f;
		else
			__b = temporary_2;

	}

}

// Static color definitions

const Color Color::Black = FromArgbf(0.0f, 0.0f, 0.0f);
const Color Color::DarkGrey = FromArgb(169, 169, 169);
const Color Color::DimGrey = FromArgb(105, 105, 105);
const Color Color::DodgerBlue = FromArgb(30, 144, 255);
const Color Color::Gainsboro = FromArgb(220, 220, 220);
const Color Color::Grey = FromArgbf(0.5f, 0.5f, 0.5f);
const Color Color::LtGrey = FromArgbf(0.7f, 0.7f, 0.7f);
const Color Color::Red = FromArgbf(1.0f, 0.0f, 0.0f);
const Color Color::Silver = FromArgb(192, 192, 192);
const Color Color::SlateGrey = FromArgb(112, 128, 144);
const Color Color::Transparent = FromArgbf(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::White = FromArgbf(1.0f, 1.0f, 1.0f);