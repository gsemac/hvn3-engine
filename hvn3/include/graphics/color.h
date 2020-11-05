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

		// CSS Colors

		static Color AliceBlue;
		static Color AntiqueWhite;
		static Color Aqua;
		static Color Aquamarine;
		static Color Azure;
		static Color Beige;
		static Color Bisque;
		static Color Black;
		static Color BlanchedAlmond;
		static Color Blue;
		static Color BlueViolet;
		static Color Brown;
		static Color BurlyWood;
		static Color CadetBlue;
		static Color Chartreuse;
		static Color Chocolate;
		static Color Coral;
		static Color CornflowerBlue;
		static Color Cornsilk;
		static Color Crimson;
		static Color Cyan;
		static Color DarkBlue;
		static Color DarkCyan;
		static Color DarkGoldenRod;
		static Color DarkGray;
		static Color DarkGreen;
		static Color DarkKhaki;
		static Color DarkMagenta;
		static Color DarkOliveGreen;
		static Color DarkOrange;
		static Color DarkOrchid;
		static Color DarkRed;
		static Color DarkSalmon;
		static Color DarkSeaGreen;
		static Color DarkSlateBlue;
		static Color DarkSlateGray;
		static Color DarkTurquoise;
		static Color DarkViolet;
		static Color DeepPink;
		static Color DeepSkyBlue;
		static Color DimGray;
		static Color DodgerBlue;
		static Color FireBrick;
		static Color FloralWhite;
		static Color ForestGreen;
		static Color Fuchsia;
		static Color Gainsboro;
		static Color GhostWhite;
		static Color Gold;
		static Color GoldenRod;
		static Color Gray;
		static Color Green;
		static Color GreenYellow;
		static Color HoneyDew;
		static Color HotPink;
		static Color IndianRed;
		static Color Indigo;
		static Color Ivory;
		static Color Khaki;
		static Color Lavender;
		static Color LavenderBlush;
		static Color LawnGreen;
		static Color LemonChiffon;
		static Color LightBlue;
		static Color LightCoral;
		static Color LightCyan;
		static Color LightGoldenRodYellow;
		static Color LightGray;
		static Color LightGreen;
		static Color LightPink;
		static Color LightSalmon;
		static Color LightSeaGreen;
		static Color LightSkyBlue;
		static Color LightSlateGray;
		static Color LightSteelBlue;
		static Color LightYellow;
		static Color Lime;
		static Color LimeGreen;
		static Color Linen;
		static Color Magenta;
		static Color Maroon;
		static Color MediumAquamarine;
		static Color MediumBlue;
		static Color MediumOrchid;
		static Color MediumPurple;
		static Color MediumSeaGreen;
		static Color MediumSlateBlue;
		static Color MediumSpringGreen;
		static Color MediumTurqoise;
		static Color MediumVioletRed;
		static Color MidnightBlue;
		static Color MintCream;
		static Color MistyRose;
		static Color Moccasin;
		static Color NavajoWhite;
		static Color Navy;
		static Color OldLace;
		static Color Olive;
		static Color OliveDrab;
		static Color Orange;
		static Color OrangeRed;
		static Color Orchid;
		static Color PaleGoldenRod;
		static Color PaleGreen;
		static Color PaleTurqoise;
		static Color PaleVioletRed;
		static Color PapayaWhip;
		static Color PeachPuff;
		static Color Peru;
		static Color Pink;
		static Color Plum;
		static Color PowderBlue;
		static Color Purple;
		static Color Red;
		static Color RosyBrown;
		static Color RoyalBlue;
		static Color SaddleBrown;
		static Color Salmon;
		static Color SandyBrown;
		static Color SeaGreen;
		static Color SeaShell;
		static Color Sienna;
		static Color Silver;
		static Color SkyBlue;
		static Color SlateBlue;
		static Color SlateGray;
		static Color Snow;
		static Color SpringGreen;
		static Color SteelBlue;
		static Color Tan;
		static Color Teal;
		static Color Thistle;
		static Color Tomato;
		static Color Turquoise;
		static Color Violet;
		static Color Wheat;
		static Color White;
		static Color WhiteSmoke;
		static Color Yellow;
		static Color YellowGreen;

	private:
		byte r, g, b, a;

	};

	Color operator*(const Color& lhs, float rhs);
	bool operator==(const Color& lhs, const Color& rhs);
	bool operator!=(const Color& lhs, const Color& rhs);
	std::ostream& operator<<(std::ostream& lhs, const Color& rhs);

}