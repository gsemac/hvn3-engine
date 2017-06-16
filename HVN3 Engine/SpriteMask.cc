#pragma once
#include <allegro5/allegro.h>
#include "SpriteMask.h"
#include "Color.h"
#include "Exception.h"

namespace hvn3 {

	SpriteMask::SpriteMask() {}
	SpriteMask::SpriteMask(Sprite* sprite) {

		__degrees = 0.0f;
		GenerateMask(sprite);

	}
	void SpriteMask::GenerateMask(Sprite* sprite) {

		// This method generates a collection of Rectangles to represent the Sprite, allowing for faster (rotated) collision checking.

		// Get the Bitmap from the Sprite.
		ALLEGRO_BITMAP* bmp = sprite->SubImage(0).AlPtr();
		int width = sprite->Width();
		int height = sprite->Height();

		// Lock the Bitmap in memory.
		al_lock_bitmap(bmp, al_get_bitmap_format(bmp), ALLEGRO_LOCK_READONLY);

		// Keep track of the Rectangles added last row, so we can expand them if possible.
		int strips_last_row = 0;

		// Generate Rectangles to represent rows of pixels.
		for (int y = 0; y <= height; ++y) {

			// Initialize variables for this strip.
			int strips_this_row = 0;
			int strip_width = 0;
			bool in_strip = false;
			Point2d<float> strip_start;

			for (int x = 0; x <= width; ++x) {

				if (!Color::IsTransparent(al_get_pixel(bmp, x, y))) {

					// If a non-transparent pixel is met, either start a new strip, or add it to the current strip.
					if (strip_width == 0)
						strip_start = Point2d<float>(x, y);
					++strip_width;

				}
				else if (strip_width > 0) {
					// If a transparent pixel is met, finalize the current strip.

					// Attempt to match the strip to an existing strip in the previous row, extending the existing strip if needed.
					bool matched_strip = false;
					for (size_t k = strips_last_row; k > 0; --k) {
						size_t ind = __strips.size() - k;
						if (strip_start.X() == __strips[ind].X() && strip_width == __strips[ind].Width()) {
							__strips[ind].Resize(__strips[ind].Width(), __strips[ind].Height() + 1);
							matched_strip = true;
							break;
						}
					}

					// If the strip wasn't matched to an existing strip, just add it to the list.
					if (!matched_strip)
						__strips.push_back(Rectangle<float>(strip_start.X(), strip_start.Y(), strip_width, 1));

					// Reset ths strip (so we can look for new ones), and increase the number of strips.
					strip_width = 0;
					strips_this_row += 1;

				}

			}

			// Set the number of strips we found in the last row.
			strips_last_row = strips_this_row;

		}

		// Unlock the Bitmap.
		al_unlock_bitmap(bmp);

	}
	std::vector<Rectangle<float>>& SpriteMask::Strips() {

		return __strips;

	}
	Rectangle<float>& SpriteMask::operator[](const size_t& strip) {

		return __strips[strip];

	}
	bool SpriteMask::Intersects(const Circle<float>& other) const {

		// Check if any strips intersect with the Circle.
		for (size_t i = 0; i < __strips.size(); ++i) {
			if (hvn3::TestIntersection(__strips[i], other))
				return true;
		}

		// No intersections found.
		return false;

	}
	bool SpriteMask::Intersects(const Rectangle<float>& other) const { throw NotImplementedException(); }
	bool SpriteMask::Intersects(const Line<float>& other) const { throw NotImplementedException(); }

}