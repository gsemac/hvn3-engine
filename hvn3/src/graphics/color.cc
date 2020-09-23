#include "core/random.h"
#include "graphics/color.h"
#include "math/math.h"
#include "utilities/convert.h"

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

		return FromHex(utilities::Convert::To<uint32_t>(hex));

	}

	Color Color::Random() {

		return Color(core::Random::Byte(), core::Random::Byte(), core::Random::Byte());

	}

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