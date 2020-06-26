#pragma once

#include <cstdint>
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
		Color(const Color& other, float alpha);

		static Color FromArgb(byte r, byte g, byte b);
		static Color FromArgb(byte r, byte g, byte b, byte alpha);
		static Color FromArgbf(float r, float g, float b);
		static Color FromArgbf(float r, float g, float b, float alpha);
		static Color FromHsl(float h, float s, float l);
		static Color FromHsl(float h, float s, float l, float alpha);
		static Color FromHex(unsigned long hex);
		static Color FromHex(const std::string& hex);
		static Color FromInt(uint32_t value);

		static Color Random();

	};

}