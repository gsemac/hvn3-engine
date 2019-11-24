#include "hvn3/graphics/Color.h"

#include "hvn3/utility/Random.h"
#include "hvn3/utility/StringUtils.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include <allegro5/allegro.h>

namespace hvn3 {

	// Public member functions

	Color::Color() : Color(0, 0, 0) {}
	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {

		_r = (r / 255.0f);
		_g = (g / 255.0f);
		_b = (b / 255.0f);
		_a = (a / 255.0f);

	}
	Color::Color(const Color& base_color, unsigned char alpha) :
		Color(base_color.R(), base_color.G(), base_color.B(), alpha) {
	}
	Color Color::FromArgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {

		return Color(r, g, b, a);

	}
	Color Color::FromArgbf(float r, float g, float b, float a) {

		Color c;
		c._r = r;
		c._g = g;
		c._b = b;
		c._a = a;
		return c;

	}
	Color Color::FromHsl(float h, float s, float l, float a) {

		Color c;
		c.HslToRgb(h, s, l);
		c._a = a;
		return c;

	}
	Color Color::FromHex(unsigned long hex) {

		float r = (hex >> 16) & 0xFF;
		float g = (hex >> 8) & 0xFF;
		float b = hex & 0xFF;
		return Color(r, g, b);

	}
	Color Color::FromHex(const std::string& hex) {
		return Color::FromHex(StringUtils::ParseHex(hex));
	}
	Color Color::FromRgbInt(unsigned int integer) {

		return Color(integer & 255, (integer >> 8) & 255, (integer >> 16) & 255);

	}
	Color Color::Random() {
		return Color::FromArgb(Random::Byte(), Random::Byte(), Random::Byte());
	}

	unsigned char Color::R() const {

		return (std::round)(_r * 255.0f);

	}
	unsigned char Color::G() const {

		return (std::round)(_g * 255.0f);

	}
	unsigned char Color::B() const {

		return (std::round)(_b * 255.0f);

	}
	float Color::Rf() const {

		return _r;

	}
	float Color::Gf() const {

		return _g;

	}
	float Color::Bf() const {

		return _b;

	}

	unsigned char Color::Alpha() const {

		return (std::round)(_a * 255.0f);

	}
	float Color::Alphaf() const {

		return _a;

	}
	float Color::Luminance() const {

		// Calculate luminance.
		float max = (std::max)({ _r, _g, _b });
		float min = (std::min)({ _r, _g, _b });
		float l = (max + min) / 2.0f;

		// Return the result.
		return l;

	}
	float Color::Hue() const {

		// Get luminance and saturation to use in calculation.
		float l = Luminance();
		float s = Saturation();

		// Calculate the min and max of the RGB values.
		float max = (std::max)({ _r, _g, _b });
		float min = (std::min)({ _r, _g, _b });

		// Calculate hue.
		float h;
		if (s > 0.0f) {
			if (_r > _g && _r > _b)
				h = (_g - _b) / (max - min);
			else if (_g > _r && _g > _b)
				h = 2.0f + (_b - _r) / (max - min);
			else
				h = 4.0f + (_r - _g) / (max - min);
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
		float max = Brightness();
		float min = (std::min)({ _r, _g, _b });

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
	float Color::Brightness() const {

		return (std::max)({ _r, _g, _b });

	}

	Color Color::Lighter(float factor) const {

		// Increase luminance.
		float h = Hue();
		float s = Saturation();
		float l = Math::Clamp(Luminance() + factor, 0.0f, 1.0f);

		// Recalculate RGB values.
		return Color::FromHsl(h, s, l);

	}
	Color Color::Darker(float factor) const {

		// Decrease luminance.
		float h = Hue();
		float s = Saturation();
		float l = Math::Clamp(Luminance() - factor, 0.0f, 1.0f);

		// Recalculate RGB values.
		return Color::FromHsl(h, s, l);

	}
	Color Color::Inverse() const {

		float h = Math::Mod(Hue() + 180.0f, 360.0f);
		float s = Saturation();
		float l = Luminance();

		return Color::FromHsl(h, s, l);

	}

	std::string Color::ToString() const {

		std::ostringstream buffer;

		buffer << std::setfill('0') << std::hex << std::left;

		buffer << "#";

		buffer << std::hex << std::setw(2) << (int)R();
		buffer << std::hex << std::setw(2) << (int)G();
		buffer << std::hex << std::setw(2) << (int)B();

		return buffer.str();

	}

	bool Color::IsTransparent(const ALLEGRO_COLOR& color) {

		return !(color.a > 0.0f);

	}
	bool Color::IsTransparent(const Color& color) {

		return color.Alpha() == 0;

	}
	bool Color::IsTranluscent(const Color& color) {

		return color.Alpha() > 0 && color.Alpha() < 255;

	}
	Color Color::Merge(const Color& color_1, const Color& color_2, float amount) {

		Color color;

		float from_c1 = 1.0f - amount;
		float from_c2 = amount;

		color._r = (color_1._r * from_c1 + color_2._r * from_c2);
		color._g = (color_1._g * from_c1 + color_2._g * from_c2);
		color._b = (color_1._b * from_c1 + color_2._b * from_c2);
		color._a = (color_1._a * from_c1 + color_2._a * from_c2);

		return color;

	}

	// Private member functions

	void Color::RgbToHsl() {

		// Logic taken from here: http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/

		float h, s, l;

		float max = (std::max)({ _r, _g, _b });
		float min = (std::min)({ _r, _g, _b });

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
			if (_r > _g && _r > _b)
				h = (_g - _b) / (max - min);
			else if (_g > _r && _g > _b)
				h = 2.0f + (_b - _r) / (max - min);
			else
				h = 4.0f + (_r - _g) / (max - min);
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

			_r = l;
			_g = l;
			_b = l;

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
				_r = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_R;
			else if (2.0f * temporary_R < 1.0f)
				_r = temporary_1;
			else if (3.0f * temporary_R < 2.0f)
				_r = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_R) * 6.0f;
			else
				_r = temporary_2;

			// Calculate G.
			if (6.0f * temporary_G < 1.0f)
				_g = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_G;
			else if (2.0f * temporary_G < 1.0f)
				_g = temporary_1;
			else if (3.0f * temporary_G < 2.0f)
				_g = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_G) * 6.0f;
			else
				_g = temporary_2;

			// Calculate B.
			if (6.0f * temporary_B < 1.0f)
				_b = temporary_2 + (temporary_1 - temporary_2) * 6.0f * temporary_B;
			else if (2.0f * temporary_B < 1.0f)
				_b = temporary_1;
			else if (3.0f * temporary_B < 2.0f)
				_b = temporary_2 + (temporary_1 - temporary_2) * ((2.0f / 3.0f) - temporary_B) * 6.0f;
			else
				_b = temporary_2;

		}

	}

	// Static color definitions

	const Color Color::AliceBlue = FromArgb(240, 248, 255);
	const Color Color::AntiqueWhite = FromArgb(250, 235, 215);
	const Color Color::Aqua = FromArgb(0, 255, 255);
	const Color Color::Aquamarine = FromArgb(127, 255, 212);
	const Color Color::Azure = FromArgb(240, 255, 255);
	const Color Color::Beige = FromArgb(245, 245, 220);
	const Color Color::Bisque = FromArgb(255, 228, 196);
	const Color Color::Black = FromArgb(0, 0, 0);
	const Color Color::BlanchedAlmond = FromArgb(255, 235, 205);
	const Color Color::Blue = FromArgb(0, 0, 255);
	const Color Color::BlueViolet = FromArgb(138, 43, 226);
	const Color Color::Brown = FromArgb(165, 42, 42);
	const Color Color::BurlyWood = FromArgb(222, 184, 135);
	const Color Color::CadetBlue = FromArgb(95, 158, 160);
	const Color Color::Chartreuse = FromArgb(127, 255, 0);
	const Color Color::Chocolate = FromArgb(210, 105, 30);
	const Color Color::Coral = FromArgb(255, 127, 80);
	const Color Color::CornFlowerBlue = FromArgb(100, 149, 237);
	const Color Color::CornSilk = FromArgb(255, 248, 220);
	const Color Color::DarkGreen = FromArgb(0, 100, 0);
	const Color Color::DarkGrey = FromArgb(169, 169, 169);
	const Color Color::DarkOrange = FromArgb(255, 140, 0);
	const Color Color::DarkSeaGreen = FromArgb(143, 188, 139);
	const Color Color::DimGrey = FromArgb(105, 105, 105);
	const Color Color::DodgerBlue = FromArgb(30, 144, 255);
	const Color Color::ForestGreen = FromArgb(34, 139, 34);
	const Color Color::Fuchsia = FromArgb(255, 0, 255);
	const Color Color::Gainsboro = FromArgb(220, 220, 220);
	const Color Color::Green = FromArgb(0, 255, 0);
	const Color Color::Grey = FromArgbf(0.5f, 0.5f, 0.5f);
	const Color Color::Orange = FromArgb(255, 165, 0);
	const Color Color::OrangeRed = FromArgb(255, 69, 0);
	const Color Color::Red = FromArgbf(1.0f, 0.0f, 0.0f);
	const Color Color::Silver = FromArgb(192, 192, 192);
	const Color Color::SlateGrey = FromArgb(112, 128, 144);
	const Color Color::Transparent = FromArgb(1, 1, 1, 0.0f);
	const Color Color::Transluscent = FromArgbf(0.5f, 0.5f, 0.5f, 0.5f);
	const Color Color::White = FromArgbf(1.0f, 1.0f, 1.0f);
	const Color Color::Yellow = FromArgb(255, 255, 0);

	// Color constants from Game Maker
	const Color Color::LtGrey = FromArgbf(0.7f, 0.7f, 0.7f);
	const Color Color::DkGrey = FromRgbInt(4210752);

	Color operator*(const Color& lhs, float rhs) {
		return Color::FromArgbf(lhs.Rf(), lhs.Gf(), lhs.Bf(), lhs.Alphaf() * rhs);
	}

	std::ostream& operator<<(std::ostream& stream, const Color& value) {

		stream << value.ToString();

		return stream;

	}

}