#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace hvn3::graphics {

	class Color {

		using byte = uint8_t;

	public:
		Color();
		Color(byte r, byte g, byte b);
		Color(byte r, byte g, byte b, byte alpha);
		Color(const Color& other);
		Color(const Color& other, byte alpha);

		byte R() const;
		byte G() const;
		byte B() const;
		byte A() const;
		float Rf() const;
		float Gf() const;
		float Bf() const;
		float Af() const;

		float Hue() const;
		float Saturation() const;
		float Luminance() const;

		Color Lighten(float amount) const;
		Color Darken(float amount) const;
		Color Invert() const;
		Color Interpolate(const Color& other, float amount) const;

		static Color FromArgb(byte r, byte g, byte b);
		static Color FromArgb(byte r, byte g, byte b, byte alpha);
		static Color FromArgbf(float r, float g, float b);
		static Color FromArgbf(float r, float g, float b, float alpha);
		static Color FromHsl(float h, float s, float l);
		static Color FromHsl(float h, float s, float l, float alpha);
		static Color FromHex(uint32_t hex);
		static Color FromHex(const std::string& hex);

		static Color Random();

	private:
		byte r, g, b, a;

	};

	Color operator*(const Color& lhs, float rhs);
	bool operator==(const Color& lhs, const Color& rhs);
	bool operator!=(const Color& lhs, const Color& rhs);
	std::ostream& operator<<(std::ostream& lhs, const Color& rhs);

}