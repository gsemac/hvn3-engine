#pragma once
#include "BitFlags.h"

struct ALLEGRO_FONT;

enum class FontOptions {
	None = 0x00,
	Scalable = 0x01,
	Monochrome = 0x02
};
ENABLE_BITFLAG_OPERATORS(FontOptions)

class Font {

public:
	Font(const char* filename, int size, FontOptions options = FontOptions::None);
	Font(Font&& other);
	~Font();

	void Scale(float scale_factor);
	bool IsScalable() const;
	int Height() const;

	ALLEGRO_FONT* AlPtr() const;

	// Returns built-in font resource.
	static Font BuiltIn();

private:
	Font();
	ALLEGRO_FONT* __font;
	bool __scalable;
	int __size, __height;
	const char* __filename;
	FontOptions __flags;

};