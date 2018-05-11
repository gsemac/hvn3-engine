#pragma once
#include "hvn3/utility/Utility.h"
#include <string>

struct ALLEGRO_COLOR;

namespace hvn3 {

	class Color {

	public:
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255u);
		Color(const Color& base_color, unsigned char alpha);

		static Color FromArgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255u);
		static Color FromArgbf(float r, float g, float b, float a = 1.0f);
		static Color FromHsl(float h, float s, float l, float a = 1.0f);
		static Color FromHex(unsigned long hex);
		static Color FromHex(const std::string& hex);
		static Color FromRgbInt(unsigned int integer);

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
		float Brightness() const;

		Color Lighter(float factor = 0.05f) const;
		Color Darker(float factor = 0.05f) const;
		Color Inverse() const;

		static bool IsTransparent(const ALLEGRO_COLOR& color);
		static bool IsTransparent(const Color& color);
		static bool IsTranluscent(const Color& color);
		static Color Merge(const Color& color_1, const Color& color_2, float amount);

		// Correspond to .NET framework colors
		static const Color AliceBlue;
		static const Color AntiqueWhite;
		static const Color Aqua;
		static const Color Aquamarine;
		static const Color Azure;
		static const Color Beige;
		static const Color Bisque;
		static const Color Black;
		static const Color BlanchedAlmond;
		static const Color Blue;
		static const Color BlueViolet;
		static const Color Brown;
		static const Color BurlyWood;
		static const Color CadetBlue;
		static const Color Chartreuse;
		static const Color Chocolate;
		static const Color Coral;
		static const Color CornFlowerBlue;
		static const Color CornSilk;
		static const Color DarkGrey;
		static const Color DarkOrange;
		static const Color DimGrey;
		static const Color DodgerBlue;
		static const Color Fuchsia;
		static const Color Gainsboro;
		static const Color Green;
		static const Color Grey;
		static const Color Orange;
		static const Color OrangeRed;
		static const Color Red;
		static const Color Silver;
		static const Color SlateGrey;
		static const Color White;
		static const Color Yellow;

		static const Color Transparent;
		static const Color Transluscent;

		// Color constants from Game Maker
		static const Color LtGrey;
		static const Color DkGrey;

	private:
		float _r, _g, _b, _a;

		void RgbToHsl();
		void HslToRgb(float h, float s, float l);

	};

}