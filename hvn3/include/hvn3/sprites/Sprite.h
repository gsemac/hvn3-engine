#pragma once
#include "hvn3/math/Point2d.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Bitmap.h"
#include <vector>
#include <string>

namespace hvn3 {

	class Sprite {

	public:
		Sprite();
		Sprite(const Graphics::Bitmap& bitmap);
		Sprite(const Graphics::Bitmap& bitmap, const PointI& origin);
		Sprite(const Graphics::Bitmap& bitmap, int ox, int oy);
		//Sprite(Sprite& other);
		Sprite(Sprite&& other);
		~Sprite();

		// Returns the width of the first sub-image, or 0 if the sprite does not contain any sub-images.
		unsigned int Width() const;
		// Returns the height of the first sub-image, or 0 if the sprite does not contain any sub-images.
		unsigned int Height() const;
		// If loaded from a sprite sheet, returns the number of sub-images in each strip. Otherwise, returns the number of sub-images.
		unsigned int StripLength() const;
		// Returns the number of subimages.
		size_t Length() const;

		// Returns the origin (offset) of the sprite.
		PointI Origin() const;
		// Sets the origin (offset) of the sprite.
		void SetOrigin(const PointI& origin);
		// Sets the origin (offset) of the sprite.
		void SetOrigin(int ox, int oy);

		// Adds a new sub-image to the sprite.
		void AddSubImage(const Graphics::Bitmap& sub_image);
		// Removes the sub-image at the given index.
		void RemoveSubImage(int sub_image);
		// Returns the sub-image at the given index.
		const Graphics::Bitmap& SubImage(int sub_image) const;
		// Returns the sub-image at the given index.
		const Graphics::Bitmap& operator[](int sub_image) const;

		// Returns a new sprite generated from a sprite sheet.
		static Sprite FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height);
		static Sprite FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height, int origin_x, int origin_y);
		static Sprite FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y);
		
		Sprite& operator=(Sprite&& other);

	private:
		std::vector<Graphics::Bitmap> _frames;
		int _ox, _oy;
		bool _using_sprite_sheet;
		unsigned int _strip_length;

		size_t _subImageToIndex(int sub_image) const;
		void _moveSpriteToThis(Sprite& other);

	};

}