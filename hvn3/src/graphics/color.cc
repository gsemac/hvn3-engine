#include "core/convert.h"
#include "core/random.h"
#include "graphics/color.h"
#include "math/math.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>

namespace hvn3::graphics {

	// Public members

	Color::Color() :
		Color(0, 0, 0) {
	}
	Color::Color(byte r, byte g, byte b) :
		Color(r, g, b, std::numeric_limits<byte>::max()) {
	}
	Color::Color(byte r, byte g, byte b, byte alpha) {

		this->r = r;
		this->g = g;
		this->b = b;
		this->a = alpha;

	}
	Color::Color(const Color& other) :
		Color(other.R(), other.G(), other.B(), other.A()) {
	}
	Color::Color(const Color& other, byte alpha) :
		Color(other.R(), other.G(), other.B(), alpha) {

	}

	Color::byte Color::R() const {

		return r;

	}
	Color::byte Color::G() const {

		return g;

	}
	Color::byte Color::B() const {

		return b;

	}
	Color::byte Color::A() const {

		return a;

	}
	float Color::Rf() const {

		return r / static_cast<float>(std::numeric_limits<byte>::max());

	}
	float Color::Gf() const {

		return g / static_cast<float>(std::numeric_limits<byte>::max());


	}
	float Color::Bf() const {

		return b / static_cast<float>(std::numeric_limits<byte>::max());


	}
	float Color::Af() const {

		return a / static_cast<float>(std::numeric_limits<byte>::max());

	}

	float Color::Hue() const {

		float luminance = Luminance();
		float saturation = Saturation();

		float minRgb = std::min({ Rf(), Gf(), Bf() });
		float maxRgb = std::max({ Rf(), Gf(), Bf() });

		float hue = 0.0f;

		if (saturation > 0.0f) {

			if (maxRgb == Rf())
				hue = (Gf() - Bf()) / (maxRgb - minRgb);
			else if (maxRgb == Gf())
				hue = 2.0f + (Bf() - Rf()) / (maxRgb - minRgb);
			else
				hue = 4.0f + (Rf() - Gf()) / (maxRgb - minRgb);

		}

		hue *= 60.0f;

		if (hue < 0.0f)
			hue += 360.0f;

		return hue;

	}
	float Color::Saturation() const {

		float luminance = Luminance();

		float minRgb = std::min({ Rf(), Gf(), Bf() });
		float maxRgb = std::max({ Rf(), Gf(), Bf() });

		float denominator;

		if (luminance <= 0.5f)
			denominator = minRgb + maxRgb;
		else
			denominator = 2.0f - minRgb - maxRgb;

		return (denominator == 0.0f) ?
			0.0f :
			(maxRgb - minRgb) / denominator;


	}
	float Color::Luminance() const {

		float minRgb = std::min({ Rf(), Gf(), Bf() });
		float maxRgb = std::max({ Rf(), Gf(), Bf() });

		float luminance = (minRgb + maxRgb) / 2.0f;

		return luminance;

	}

	Color Color::Lighten(float amount) const {

		float hue = Hue();
		float saturation = Saturation();
		float luminance = math::Clamp(Luminance() + amount, 0.0f, 1.0f);

		return Color::FromHsl(hue, saturation, luminance);

	}
	Color Color::Darken(float amount) const {

		return Lighten(-amount);

	}
	Color Color::Invert() const {

		float hue = math::Mod(Hue() + 180.0f, 360.0f);
		float saturation = Saturation();
		float luminance = Luminance();

		return Color::FromHsl(hue, saturation, luminance);

	}
	Color Color::Interpolate(const Color& other, float amount) const {

		float amountFromThis = 1.0f - amount;
		float amountFromOther = amount;

		float r = Rf() * amountFromThis + other.Rf() * amountFromOther;
		float g = Gf() * amountFromThis + other.Gf() * amountFromOther;
		float b = Bf() * amountFromThis + other.Bf() * amountFromOther;
		float a = Af() * amountFromThis + other.Af() * amountFromOther;

		return FromArgbf(r, g, b, a);

	}

	Color Color::FromArgb(byte r, byte g, byte b) {

		return FromArgb(r, g, b, std::numeric_limits<byte>::max());

	}
	Color Color::FromArgb(byte r, byte g, byte b, byte alpha) {

		return Color(r, g, b, alpha);

	}
	Color Color::FromArgbf(float r, float g, float b) {

		return FromArgbf(r, g, b, 1.0f);

	}
	Color Color::FromArgbf(float r, float g, float b, float alpha) {

		byte rB = static_cast<byte>(r * std::numeric_limits<byte>::max());
		byte gB = static_cast<byte>(g * std::numeric_limits<byte>::max());
		byte bB = static_cast<byte>(b * std::numeric_limits<byte>::max());
		byte aB = static_cast<byte>(alpha * std::numeric_limits<byte>::max());

		return FromArgb(rB, gB, bB, aB);

	}
	Color Color::FromHsl(float h, float s, float l) {

		return FromHsl(h, s, l, 1.0f);

	}
	Color Color::FromHsl(float h, float s, float l, float alpha) {

		// The HSL to RGB logic is adapted from the process described here:
		// https://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/

		if (s == 0.0f) {

			// No saturation means that we have a shade of grey.

			return FromArgbf(l, l, l, alpha);

		}
		else {

			// Create some temporary variables to make the following formulas easier to read.

			float temp1, temp2;

			if (l < 0.5f)
				temp1 = l * (1.0f + s);
			else
				temp1 = (l + s) - (l * s);

			temp2 = 2.0f * l - temp1;

			// Convert the hue from degrees to a percentage (of a circle).

			float huePercentage = h / 360.0f;

			// Calculate each of the RGB values.

			float tempR = huePercentage + (1.0f / 3.0f);
			float tempG = huePercentage;
			float tempB = huePercentage - (1.0f / 3.0f);

			if (tempR < 0.0f)
				tempR += 1.0f;
			else if (tempR > 1.0f)
				tempR -= 1.0f;

			if (tempG < 0.0f)
				tempG += 1.0f;
			else if (tempG > 1.0f)
				tempG -= 1.0f;

			if (tempB < 0.0f)
				tempB += 1.0f;
			else if (tempB > 1.0f)
				tempB -= 1.0f;

			float r, g, b;

			if (6.0f * tempR < 1.0f)
				r = temp2 + (temp1 - temp2) * 6.0f * tempR;
			else if (2.0f * tempR < 1.0f)
				r = temp1;
			else if (3.0f * tempR < 2.0f)
				r = temp2 + (temp1 - temp2) * ((2.0f / 3.0f) - tempR) * 6.0f;
			else
				r = temp2;

			if (6.0f * tempG < 1.0f)
				g = temp2 + (temp1 - temp2) * 6.0f * tempG;
			else if (2.0f * tempG < 1.0f)
				g = temp1;
			else if (3.0f * tempG < 2.0f)
				g = temp2 + (temp1 - temp2) * ((2.0f / 3.0f) - tempG) * 6.0f;
			else
				g = temp2;

			if (6.0f * tempB < 1.0f)
				b = temp2 + (temp1 - temp2) * 6.0f * tempB;
			else if (2.0f * tempB < 1.0f)
				b = temp1;
			else if (3.0f * tempB < 2.0f)
				b = temp2 + (temp1 - temp2) * ((2.0f / 3.0f) - tempB) * 6.0f;
			else
				b = temp2;

			return FromArgbf(r, g, b, alpha);

		}

	}
	Color Color::FromHex(uint32_t hex) {

		byte r = (hex >> 16) & 0xFF;
		byte g = (hex >> 8) & 0xFF;
		byte b = hex & 0xFF;

		return Color(r, g, b);

	}
	Color Color::FromHex(const std::string& hex) {

		return FromHex(core::Convert::To<uint32_t>(hex));

	}

	Color Color::Random() {

		return Color(core::Random::Byte(), core::Random::Byte(), core::Random::Byte());

	}

	Color Color::AliceBlue = Color::FromHex(0xF0F8FF);
	Color Color::AntiqueWhite = Color::FromHex(0xFAEBD7);
	Color Color::Aqua = Color::FromHex(0x00FFFF);
	Color Color::Aquamarine = Color::FromHex(0x7FFFD4);
	Color Color::Azure = Color::FromHex(0xF0FFFF);
	Color Color::Beige = Color::FromHex(0xF5F5DC);
	Color Color::Bisque = Color::FromHex(0xFFE4C4);
	Color Color::Black = Color::FromHex(0xFFE4C4);
	Color Color::BlanchedAlmond = Color::FromHex(0xFFEBCD);
	Color Color::Blue = Color::FromHex(0x0000FF);
	Color Color::BlueViolet = Color::FromHex(0x8A2BE2);
	Color Color::Brown = Color::FromHex(0xA52A2A);
	Color Color::BurlyWood = Color::FromHex(0xDEB887);
	Color Color::CadetBlue = Color::FromHex(0x5F9EA0);
	Color Color::Chartreuse = Color::FromHex(0x7FFF00);
	Color Color::Chocolate = Color::FromHex(0xD2691E);
	Color Color::Coral = Color::FromHex(0xFF7F50);
	Color Color::CornflowerBlue = Color::FromHex(0x6495ED);
	Color Color::Cornsilk = Color::FromHex(0xFFF8DC);
	Color Color::Crimson = Color::FromHex(0xDC143C);
	Color Color::Cyan = Color::FromHex(0x00FFFF);
	Color Color::DarkBlue = Color::FromHex(0x00008B);
	Color Color::DarkCyan = Color::FromHex(0x008B8B);
	Color Color::DarkGoldenRod = Color::FromHex(0xB8860B);
	Color Color::DarkGray = Color::FromHex(0xA9A9A9);
	Color Color::DarkGreen = Color::FromHex(0x006400);
	Color Color::DarkKhaki = Color::FromHex(0xBDB76B);
	Color Color::DarkMagenta = Color::FromHex(0x8B008B);
	Color Color::DarkOliveGreen = Color::FromHex(0x556B2F);
	Color Color::DarkOrange = Color::FromHex(0xFF8C00);
	Color Color::DarkOrchid = Color::FromHex(0x9932CC);
	Color Color::DarkRed = Color::FromHex(0x8B0000);
	Color Color::DarkSalmon = Color::FromHex(0x8B0000);
	Color Color::DarkSeaGreen = Color::FromHex(0x8FBC8F);
	Color Color::DarkSlateBlue = Color::FromHex(0x483D8B);
	Color Color::DarkSlateGray = Color::FromHex(0x2F4F4F);
	Color Color::DarkTurquoise = Color::FromHex(0x00CED1);
	Color Color::DarkViolet = Color::FromHex(0x9400D3);
	Color Color::DeepPink = Color::FromHex(0x9400D3);
	Color Color::DeepSkyBlue = Color::FromHex(0x00BFFF);
	Color Color::DimGray = Color::FromHex(0x696969);
	Color Color::DodgerBlue = Color::FromHex(0x1E90FF);
	Color Color::FireBrick = Color::FromHex(0xB22222);
	Color Color::FloralWhite = Color::FromHex(0xFFFAF0);
	Color Color::ForestGreen = Color::FromHex(0x228B22);
	Color Color::Fuchsia = Color::FromHex(0xFF00FF);
	Color Color::Gainsboro = Color::FromHex(0xDCDCDC);
	Color Color::GhostWhite = Color::FromHex(0xF8F8FF);
	Color Color::Gold = Color::FromHex(0xFFD700);
	Color Color::GoldenRod = Color::FromHex(0xDAA520);
	Color Color::Gray = Color::FromHex(0x808080);
	Color Color::Green = Color::FromHex(0x008000);
	Color Color::GreenYellow = Color::FromHex(0xADFF2F);
	Color Color::HoneyDew = Color::FromHex(0xF0FFF0);
	Color Color::HotPink = Color::FromHex(0xFF69B4);
	Color Color::IndianRed = Color::FromHex(0xCD5C5C);
	Color Color::Indigo = Color::FromHex(0x4B0082);
	Color Color::Ivory = Color::FromHex(0xFFFFF0);
	Color Color::Khaki = Color::FromHex(0xF0E68C);
	Color Color::Lavender = Color::FromHex(0xE6E6FA);
	Color Color::LavenderBlush = Color::FromHex(0xFFF0F5);
	Color Color::LawnGreen = Color::FromHex(0x7CFC00);
	Color Color::LemonChiffon = Color::FromHex(0xFFFACD);
	Color Color::LightBlue = Color::FromHex(0xADD8E6);
	Color Color::LightCoral = Color::FromHex(0xF08080);
	Color Color::LightCyan = Color::FromHex(0xE0FFFF);
	Color Color::LightGoldenRodYellow = Color::FromHex(0xFAFAD2);
	Color Color::LightGray = Color::FromHex(0xD3D3D3);
	Color Color::LightGreen = Color::FromHex(0x90EE90);
	Color Color::LightPink = Color::FromHex(0xFFB6C1);
	Color Color::LightSalmon = Color::FromHex(0xFFA07A);
	Color Color::LightSeaGreen = Color::FromHex(0x20B2AA);
	Color Color::LightSkyBlue = Color::FromHex(0x87CEFA);
	Color Color::LightSlateGray = Color::FromHex(0x778899);
	Color Color::LightSteelBlue = Color::FromHex(0xB0C4DE);
	Color Color::LightYellow = Color::FromHex(0xFFFFE0);
	Color Color::Lime = Color::FromHex(0x00FF00);
	Color Color::LimeGreen = Color::FromHex(0x32CD32);
	Color Color::Linen = Color::FromHex(0xFAF0E6);
	Color Color::Magenta = Color::FromHex(0xFF00FF);
	Color Color::Maroon = Color::FromHex(0x800000);
	Color Color::MediumAquamarine = Color::FromHex(0x66CDAA);
	Color Color::MediumBlue = Color::FromHex(0x0000CD);
	Color Color::MediumOrchid = Color::FromHex(0xBA55D3);
	Color Color::MediumPurple = Color::FromHex(0x9370D8);
	Color Color::MediumSeaGreen = Color::FromHex(0x3CB371);
	Color Color::MediumSlateBlue = Color::FromHex(0x7B68EE);
	Color Color::MediumSpringGreen = Color::FromHex(0x00FA9A);
	Color Color::MediumTurqoise = Color::FromHex(0x48D1CC);
	Color Color::MediumVioletRed = Color::FromHex(0xC71585);
	Color Color::MidnightBlue = Color::FromHex(0x191970);
	Color Color::MintCream = Color::FromHex(0xF5FFFA);
	Color Color::MistyRose = Color::FromHex(0xFFE4E1);
	Color Color::Moccasin = Color::FromHex(0xFFE4B5);
	Color Color::NavajoWhite = Color::FromHex(0xFFDEAD);
	Color Color::Navy = Color::FromHex(0x000080);
	Color Color::OldLace = Color::FromHex(0xFDF5E6);
	Color Color::Olive = Color::FromHex(0x808000);
	Color Color::OliveDrab = Color::FromHex(0x6B8E23);
	Color Color::Orange = Color::FromHex(0xFFA500);
	Color Color::OrangeRed = Color::FromHex(0xFF4500);
	Color Color::Orchid = Color::FromHex(0xDA70D6);
	Color Color::PaleGoldenRod = Color::FromHex(0xEEE8AA);
	Color Color::PaleGreen = Color::FromHex(0x98FB98);
	Color Color::PaleTurqoise = Color::FromHex(0xAFEEEE);
	Color Color::PaleVioletRed = Color::FromHex(0xD87093);
	Color Color::PapayaWhip = Color::FromHex(0xFFEFD5);
	Color Color::PeachPuff = Color::FromHex(0xFFDAB9);
	Color Color::Peru = Color::FromHex(0xCD853F);
	Color Color::Pink = Color::FromHex(0xFFC0CB);
	Color Color::Plum = Color::FromHex(0xDDA0DD);
	Color Color::PowderBlue = Color::FromHex(0xB0E0E6);
	Color Color::Purple = Color::FromHex(0x800080);
	Color Color::Red = Color::FromHex(0xFF0000);
	Color Color::RosyBrown = Color::FromHex(0xBC8F8F);
	Color Color::RoyalBlue = Color::FromHex(0x4169E1);
	Color Color::SaddleBrown = Color::FromHex(0x8B4513);
	Color Color::Salmon = Color::FromHex(0xFA8072);
	Color Color::SandyBrown = Color::FromHex(0xF4A460);
	Color Color::SeaGreen = Color::FromHex(0xF4A460);
	Color Color::SeaShell = Color::FromHex(0xFFF5EE);
	Color Color::Sienna = Color::FromHex(0xA0522D);
	Color Color::Silver = Color::FromHex(0xC0C0C0);
	Color Color::SkyBlue = Color::FromHex(0x87CEEB);
	Color Color::SlateBlue = Color::FromHex(0x6A5ACD);
	Color Color::SlateGray = Color::FromHex(0x708090);
	Color Color::Snow = Color::FromHex(0xFFFAFA);
	Color Color::SpringGreen = Color::FromHex(0x00FF7F);
	Color Color::SteelBlue = Color::FromHex(0x4682B4);
	Color Color::Tan = Color::FromHex(0xD2B48C);
	Color Color::Teal = Color::FromHex(0x008080);
	Color Color::Thistle = Color::FromHex(0xD8BFD8);
	Color Color::Tomato = Color::FromHex(0xFF6347);
	Color Color::Turquoise = Color::FromHex(0x40E0D0);
	Color Color::Violet = Color::FromHex(0xEE82EE);
	Color Color::Wheat = Color::FromHex(0xF5DEB3);
	Color Color::White = Color::FromHex(0xFFFFFF);
	Color Color::WhiteSmoke = Color::FromHex(0xF5F5F5);
	Color Color::Yellow = Color::FromHex(0xFFFF00);
	Color Color::YellowGreen = Color::FromHex(0x9ACD32);

	Color operator*(const Color& lhs, float rhs) {

		return Color::FromArgbf(lhs.Rf(), lhs.Gf(), lhs.Bf(), lhs.Af() * rhs);

	}
	bool operator==(const Color& lhs, const Color& rhs) {

		return lhs.R() == rhs.R() &&
			lhs.G() == rhs.G() &&
			lhs.B() == rhs.B() &&
			lhs.A() == rhs.A();

	}
	bool operator!=(const Color& lhs, const Color& rhs) {

		return !(lhs == rhs);

	}
	std::ostream& operator<<(std::ostream& lhs, const Color& rhs) {

		std::ios_base::fmtflags originalFormatFlags(lhs.flags());

		lhs << std::setfill('0') << std::hex << std::left;

		lhs << '#';

		lhs << std::hex << std::setw(2) << rhs.R();
		lhs << std::hex << std::setw(2) << rhs.G();
		lhs << std::hex << std::setw(2) << rhs.B();

		lhs.flags(originalFormatFlags);

		return lhs;

	}

	// Private members

}